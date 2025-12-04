#include "SamplingProfiler.h"
#include <thread>
#include <chrono>
#include <DbgHelp.h>
#include "Log.h"
#include <assert.h>

#define TLOG(X) resultFile << X << std::endl; std::cout << X << std::endl; resultFile.close(); resultFile.open("SamplingResults.txt", std::ios::app);

using namespace std::chrono_literals;
void SamplingProfiler::SampleThread()
{
	std::ofstream resultFile; 
	resultFile.open("SamplingResults.txt");
	try
	{
		while (m_loopCondition)
		{
			DWORD suspend = SuspendThread(m_threadHandle);

			if (suspend != 0)
			{
				TLOG("Couldnt suspend thread.");
				return;
			}

			if (!GetThreadContext(m_threadHandle, &m_context))
			{
				TLOG("Couldnt get thread context.");
				if (ResumeThread(m_threadHandle) == -1)
				{
					TLOG("Couldnt resume thread");
				}
				return;
			}
			

#ifdef _WIN64
			void* funcPtr = (void*)m_context.Rip;
#else
			void* funcPtr = (void*)m_context.Eip();
#endif
			
			ResumeThread(m_threadHandle);

			if (m_funcPtrs.find(funcPtr) == m_funcPtrs.end())
				m_funcPtrs.insert({ funcPtr, 0 });

			m_funcPtrs[funcPtr]++;

			
			std::this_thread::sleep_for(m_captureIntervalMs);
		}
	}
	catch (std::exception e)
	{
		TLOG(e.what());
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
	if (SymFromAddr(GetCurrentProcess(), (DWORD64)rip, 0, symbol))
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


void SamplingProfiler::Start()
{
	m_loopCondition = true;
	m_samplingThread = std::thread(&SamplingProfiler::SampleThread, this);
}

void SamplingProfiler::End()
{
	m_loopCondition = false;
	if (m_samplingThread.joinable())
	{
		m_samplingThread.join();
	}

	HANDLE process = GetCurrentProcess();

	SymSetOptions(SYMOPT_UNDNAME | SYMOPT_DEFERRED_LOADS);
	SymInitialize(process, NULL, TRUE);
	char buffer[MAX_PATH];
	GetCurrentDirectoryA(MAX_PATH, buffer);

	SymSetSearchPath(process, buffer);

	// as we do not know the size of the name of the function, we need to just allocate a lot
	SYMBOL_INFO* symbol = (SYMBOL_INFO*)malloc(sizeof(SYMBOL_INFO) + 256);
	symbol->SizeOfStruct = sizeof(SYMBOL_INFO);
	symbol->MaxNameLen = 255;

	// collapse RIPs to function start addresses
	std::map<std::string, int> countByName;

	for (std::pair<void*, int> item : m_funcPtrs)
	{
		std::stringstream nameSS;
		void* funcStart = GetFuncStartPtr(item.first);
		memset(symbol->Name, 0, symbol->MaxNameLen);
		if (SymFromAddr(process, (ULONG64)funcStart, 0, symbol))
		{
			nameSS << symbol->Name;
		}
		else
		{
			DWORD err = GetLastError();
			nameSS << "Error: " << "<Unknown> at address" << item.first;
		}

		std::string name = nameSS.str();

		if (countByName.find(name) == countByName.end())
			countByName.insert({ name, 0 });

		// aggregate counts
		countByName[name] += item.second; 
	}

	std::stringstream out;
	for (std::pair<std::string, int> item : countByName)
	{
		std::string funcName = item.first;
		int callCount = item.second;

		out << "Function: " << funcName << " was caught " << callCount << " time(s).\n";
	}
	std::ofstream resultFile;
	resultFile.open("SamplingResults.txt", std::ios::app);
	TLOG(out.str());

	free(symbol);
	SymCleanup(process);
}

SamplingProfiler::SamplingProfiler(DWORD threadID, int captureIntervalMs) : m_context({})
{
	m_threadHandle = OpenThread(THREAD_SUSPEND_RESUME | THREAD_GET_CONTEXT, FALSE, threadID);
	
	m_context.ContextFlags = CONTEXT_FULL;

	m_captureIntervalMs = std::chrono::milliseconds(captureIntervalMs);
	
}
