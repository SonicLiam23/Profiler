#pragma once
#include <windows.h>
#include <map>
#include <chrono>
#include <thread>

class SamplingProfiler
{
	CONTEXT m_context;
	HANDLE m_threadHandle;
	std::chrono::milliseconds m_captureIntervalMs;
	std::thread m_samplingThread;
	std::map<void*, int> m_funcPtrs;
	void SampleThread();
	bool m_loopCondition;
	void* GetFuncStartPtr(void* rip);
	
public:
	void Start();
	void End();
	SamplingProfiler(DWORD threadID, int captureIntervalMs);
};

