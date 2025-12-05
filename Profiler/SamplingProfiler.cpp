#include "SamplingProfiler.h"
#include <thread>
#include <chrono>
#include <map>
#include <windows.h>
#include "Log.h"
#include <assert.h>
#include <stack>

#define TLOG(X) resultFile << X << std::endl; std::cout << X << std::endl; resultFile.close(); resultFile.open("SamplingResults.txt", std::ios::app);

using namespace std::chrono_literals;
bool SamplingProfiler::CaptureStack(CurrCallStack& outStack)
{
	outStack.count = 0;


	CONTEXT ctx = {};
	ctx.ContextFlags = CONTEXT_FULL;

	if (SuspendThread(m_threadHandle) == (DWORD)-1)
		return false;

	if (!GetThreadContext(m_threadHandle, &ctx))
	{
		ResumeThread(m_threadHandle);
		return false;
	}

	STACKFRAME64 frame = {};
	frame.AddrPC.Offset = ctx.Rip;
	frame.AddrPC.Mode = AddrModeFlat;
	frame.AddrFrame.Offset = ctx.Rbp;
	frame.AddrFrame.Mode = AddrModeFlat;
	frame.AddrStack.Offset = ctx.Rsp;
	frame.AddrStack.Mode = AddrModeFlat;

	// avoid locking the thread, push_back allocates -> main thread, which is suspended -> deadlock!
	for (int i = 0; i < MAX_FRAMES; ++i)
	{
		BOOL ok = StackWalk64(
			IMAGE_FILE_MACHINE_AMD64,
			m_process,
			m_threadHandle,
			&frame,
			&ctx,
			NULL,
			SymFunctionTableAccess64,
			SymGetModuleBase64,
			NULL
		);

		if (!ok || frame.AddrPC.Offset == 0)
			break;

		uint64_t PC = frame.AddrPC.Offset;

		// top 16 bits must be all 0 or all 1 on x64
		uint64_t top = PC >> 48;
		if (PC == 0 || top == 0xCCCC || PC == 0xFFFFFFFFFFFFFFFFULL) // quick guards
			break;

		//// if moduleBase == 0 stop
		//DWORD64 moduleBase = SymGetModuleBase64(m_process, (DWORD64)PC);
		//if (moduleBase == 0)
		//{
		//	// If no module is found technically i could just contiue but its safer to just stop
		//	break;
		//}

		outStack.frames[outStack.count++] = (void*)frame.AddrPC.Offset;
	}
	DWORD resumeRes = ResumeThread(m_threadHandle);
	if (resumeRes == (DWORD)-1)
	{
		// catastrophic
		return false;
	}
	return true;
}

void SamplingProfiler::SampleThread()
{
	std::ofstream resultFile; 
	resultFile.open("SamplingResults.txt");
	while (m_loopCondition)
	{
		CurrCallStack sample;
		sample.count = 0;

		CurrCallStack thisStack;
		if (CaptureStack(thisStack))
		{
			// copy only valid frames
			for (int i = 0; i < thisStack.count && sample.count < MAX_FRAMES; ++i)
			{
				sample.frames[sample.count++] = thisStack.frames[i];
			}

			m_callStacks.push_back(sample);
		}

		std::this_thread::sleep_for(m_captureIntervalMs);
	}
	resultFile.close();
}

// i think there is quite a bit of explaining...
// so this isn't actually run at sample, only at the end when im resolving function names.
// because of the case where im in a function with no unwind metadata (e.g leaf functions)
// i need to call SymFromAddr(), which is slow so i do NOT want to be doing it during the
// sampling. hence why i call it at the end.
// so "funcPtrs" is slightly misleading, as it *could* be pointing to a line inside the function
// but this IS resolved in here, called in End()!
//
// tl;dr not ran during sampling but after, so I'm not too bothered about its poor performance.
void* SamplingProfiler::GetFuncStartPtr(void* rip)
{
	DWORD64 base;

	PRUNTIME_FUNCTION functionPtr = RtlLookupFunctionEntry((DWORD64)rip, &base, nullptr);
	if (functionPtr)
		return (void*)(base + functionPtr->BeginAddress);

	// fallback to symbols if there is no unwind info
	char buffer[MAX_PATH];
	SYMBOL_INFO* symbol = (SYMBOL_INFO*)malloc(sizeof(SYMBOL_INFO) + 256);
	symbol->SizeOfStruct = sizeof(SYMBOL_INFO);
	symbol->MaxNameLen = 255;

	void* funcAddress;
	if (SymFromAddr(m_process, (DWORD64)rip, 0, symbol))
	{
		funcAddress = (void*)symbol->Address;
	}
	else
	{
		// if all else fails, default to using the rip
		funcAddress = rip;
	}

	free(symbol);
	return funcAddress;

}

void SamplingProfiler::PrintTreeFromNode(const FuncNode& node, PSYMBOL_INFO symbol, int totalHits, std::vector<std::string> currentPath, int depth)
{
	DWORD64 displacement = 0;
	std::string funcName;

	// Resolve symbol name if possible
	if (node.functionPtr && SymFromAddr(m_process, (DWORD64)node.functionPtr, &displacement, symbol))
		funcName = symbol->Name;
	else if (node.functionPtr)
		funcName = "0x" + std::to_string((uintptr_t)node.functionPtr);
	else
		funcName = "[root]";

	currentPath.push_back(funcName);

	// Collapse trivial / system frames, or ignore them (makes the graph pretty hard to read)
	if (node.hitCount < 5 && depth > 10)
	{
#ifdef SAMPLE_ALL
		std::stringstream out;
		out << std::string(depth * 2, ' ') << funcName << ": "
			<< node.hitCount << " hits (" << (double)node.hitCount / (double)totalHits * 100.0 << "%)";
		TLOG(out.str());
#endif
		return;
	}

	// Print current node
	if (depth > 0) // skip root itself
	{
		std::stringstream out;
		out << std::string(depth * 2, ' ') << funcName
			<< " : " << node.hitCount
			<< " hits (" << (double)node.hitCount / (double)totalHits * 100.0 << "%)";
		TLOG(out.str());
	}

	// flamegraph style, at the end of each "branch"
	if (node.children.empty() && depth > 0)
	{
		std::stringstream flatOut;
		for (size_t i = 0; i < currentPath.size(); ++i)
		{
			flatOut << currentPath[i];
			if (i + 1 < currentPath.size())
				flatOut << " -> ";
		}
		flatOut << " : " << node.hitCount << " hits (" << (double)node.hitCount / totalHits * 100.0 << "%)";
		TLOG(flatOut.str());
	}

	for (std::pair<void* const, FuncNode> child : node.children)
		PrintTreeFromNode(child.second, symbol, node.hitCount, currentPath, depth + 1);

}


void SamplingProfiler::Start()
{
	m_loopCondition = true;
	if (!SymInitialize(m_process, nullptr, TRUE)) {
		DWORD err = GetLastError();
		// handle error / log err
	}
	SymSetOptions(SYMOPT_DEFERRED_LOADS | SYMOPT_UNDNAME);

	m_samplingThread = std::thread(&SamplingProfiler::SampleThread, this);
}

void SamplingProfiler::End()
{
	m_loopCondition = false;
	if (m_samplingThread.joinable())
	{
		m_samplingThread.join();
	}

	m_stackRoot = FuncNode();

	for (const CurrCallStack& stack : m_callStacks)
	{
		FuncNode* node = &m_stackRoot;
		
		// the last index is "main" (or whatever external code gets main)
		for (int i = stack.count - 1; i >= 0; --i)
		{
			void* funcStart = GetFuncStartPtr(stack.frames[i]);

			DWORD64 moduleBase = SymGetModuleBase64(m_process, (DWORD64)funcStart);
			if (moduleBase) {
				char moduleName[MAX_PATH] = {};
				if (GetModuleFileNameA((HMODULE)moduleBase, moduleName, MAX_PATH)) {
					std::string modStr = moduleName;
					if (modStr.find("ucrt") != std::string::npos ||
						modStr.find("kernel32") != std::string::npos ||
						modStr.find("ntdll") != std::string::npos)
					{
						continue; // skip this frame
					}
				}
			}
			node = &node->children[funcStart];
			node->functionPtr = funcStart;
			node->hitCount++;
		}
	}

	const int MAX_NAME = 1024;
	char buffer[sizeof(SYMBOL_INFO) + MAX_NAME];
	PSYMBOL_INFO symbol = reinterpret_cast<PSYMBOL_INFO>(buffer);
	symbol->SizeOfStruct = sizeof(SYMBOL_INFO);
	symbol->MaxNameLen = MAX_NAME;

	resultFile.open("SamplingResults.txt", std::ios::app);
	PrintTreeFromNode(m_stackRoot, symbol, 0);


}

SamplingProfiler::SamplingProfiler(DWORD threadID, int captureIntervalMs) : m_context({}), m_frame({}), m_process(GetCurrentProcess())
{
	m_threadHandle = OpenThread(THREAD_SUSPEND_RESUME | THREAD_GET_CONTEXT, FALSE, threadID);
	
	m_context.ContextFlags = CONTEXT_FULL;

	m_captureIntervalMs = std::chrono::milliseconds(captureIntervalMs);
	
}
