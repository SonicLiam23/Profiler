#pragma once
#include <chrono>
#define CONCAT2(a, b)  a##b
#define CONCAT(a, b) CONCAT2(a, b)
#define TIME_THIS_RETURN(x, y) ScopedTimer CONCAT(scopedTimer, __LINE__)(x, y)
#define TIME_THIS(x) ScopedTimer CONCAT(scopedTimer, __LINE__)(x)


class Timer
{
protected:
	std::chrono::steady_clock::time_point start;
	const char* name;
	Timer(const char* pName) : name(pName) {}
	Timer(const Timer&) = default;
};

struct ScopedTimer : public Timer
{
	ScopedTimer(const char* pPame, float* pTime = nullptr, bool pOutput = true);
	~ScopedTimer();

private:
	float* time;
	void* operator new(size_t) = delete;
	void* operator new[](size_t) = delete;
	bool output;
};

struct ManualTimer : public Timer
{
	ManualTimer(const char* pName);
	std::chrono::steady_clock::time_point Start();
	std::chrono::steady_clock::time_point Stop(float* getTime = nullptr, bool output = true);
};
