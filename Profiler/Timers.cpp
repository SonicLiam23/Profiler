#include "Timers.h"
#include <chrono>
#include "Log.h"
#include <sstream>





/// SCOPED TIMER
ScopedTimer::ScopedTimer(const char* pName, float* pTime, bool pOutput) : Timer(pName), time(pTime), output(pOutput)
{
	start = std::chrono::steady_clock::now();
}

ScopedTimer::~ScopedTimer()
{
	float timeDiff = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start).count();

	if (output)
	{
		std::stringstream ss;
		ss << name << " took " << timeDiff << " microseconds to complete.";
		LOGTEXTM(ss.str());
	}


	*time = timeDiff;
}





// MANUAL TIMER
ManualTimer::ManualTimer(const char* pName) : Timer(pName)
{
	name = pName;
}

std::chrono::steady_clock::time_point ManualTimer::Start()
{
	start = std::chrono::steady_clock::now();
	return start;
}

std::chrono::steady_clock::time_point ManualTimer::Stop(float* getTime, bool output)
{
	std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
	std::chrono::duration<float, std::milli> diff = now - start; // float milliseconds
	float timeDiff = diff.count();
	if (getTime)
		*getTime = timeDiff;
	if (output)
	{
		std::stringstream ss;
		ss << name << " took " << timeDiff << " milliseconds to complete.";
		LOGTEXTM(ss.str());

	}


	return now;
}