#pragma once
#include <windows.h>
#include <DbgHelp.h>
#include <vector>
#include <chrono>
#include <thread>
#include "FuncNode.h";
#include <fstream>
// the max depth of 1 call stack
constexpr int MAX_FRAMES = 64;


struct CurrCallStack
{
	int count;
	void* frames[MAX_FRAMES];

	CurrCallStack() : count(0) {}
};

class SamplingProfiler
{
	CONTEXT m_context;
	HANDLE m_threadHandle;
	HANDLE m_process;
	STACKFRAME64 m_frame;
	FuncNode m_stackRoot;
	std::ofstream resultFile;

	std::chrono::milliseconds m_captureIntervalMs;
	std::thread m_samplingThread;
	std::vector<CurrCallStack> m_callStacks;
	bool CaptureStack(CurrCallStack& outStack);
	void SampleThread();
	bool m_loopCondition;
	void* GetFuncStartPtr(void* rip);
	void PrintTreeFromNode(const FuncNode& node, PSYMBOL_INFO symbol, int totalHits, std::vector<std::string> currentPath = {}, int depth = 0);
	
public:
	void Start();
	void End();
	SamplingProfiler(DWORD threadID, int captureIntervalMs);
};

