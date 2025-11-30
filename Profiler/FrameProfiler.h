#pragma once
#include <chrono>
#include <deque>

struct FrameInfo
{
	float deltaTime; //ms
	std::chrono::steady_clock::time_point timestamp;
};

class FrameProfiler
{
public:
	FrameProfiler(float profileWindow) : m_profileWindowSecs(profileWindow), m_sumDeltaTimeMs(0.0f), m_sumRecipDeltaTimeMs(0.0f) {}

	void AddFrame(float deltaTime, std::chrono::steady_clock::time_point now);

	float GetAverageDelta() const;

	// mean
	float GetAverageFPS() const;

	float GetHarmonicMeanFPS() const;

	float GetAvgLowOnePercent() const;

	float GetMinFPS() const;


private:
	std::deque<FrameInfo> m_frames;
	float m_profileWindowSecs;
	float m_sumDeltaTimeMs;
	float m_sumRecipDeltaTimeMs;
};

