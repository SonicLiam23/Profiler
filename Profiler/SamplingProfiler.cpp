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


void SamplingProfiler::Start()
{
	//LOGTEXTM("SAMPLING START");
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
	SymInitialize(process, NULL, TRUE);
	char buffer[MAX_PATH];
	GetCurrentDirectoryA(MAX_PATH, buffer);

	SymSetSearchPath(process, buffer);

	// as we do not know the size of the name of the function, we need to just allocate a lot
	SYMBOL_INFO* symbol = (SYMBOL_INFO*)malloc(sizeof(SYMBOL_INFO) + 256);
	symbol->SizeOfStruct = sizeof(SYMBOL_INFO);
	symbol->MaxNameLen = 255;

	std::stringstream out;
	for (std::pair<void*, int> item : m_funcPtrs)
	{
		void* funcPtr = item.first;
		int callCount = item.second;

		memset(symbol->Name, 0, symbol->MaxNameLen);

		if (SymFromAddr(process, (ULONG64)funcPtr, 0, symbol))
		{
			out << "Function: " << symbol->Name << " was caught " << callCount << " times.";
		}
		else
		{
			DWORD err = GetLastError();
			out << "Function: <Unknown> at address " << funcPtr << " caught " << callCount << " times. Error Code " << err;
		}

		out << "\n";

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
	
	m_context.ContextFlags = CONTEXT_ALL;

	m_captureIntervalMs = std::chrono::milliseconds(captureIntervalMs);
	
}
