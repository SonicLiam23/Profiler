#pragma once
#include <windows.h>
#include <vector>
#include <chrono>
#include <thread>

class SamplingProfiler
{
	CONTEXT m_context;
	HANDLE m_threadHandle;
	std::chrono::milliseconds m_captureIntervalMs;
	std::thread m_samplingThread;
	std::vector<std::pair<void*, int>> m_funcPtrs;
	void SampleThread();
	bool m_loopCondition;
	
public:
	void Start();
	void End();
	SamplingProfiler(DWORD threadID, int captureIntervalMs);
};

