#include "FrameProfiler.h"
#include <vector>
#include <algorithm>
#define ZEROF 0.0f

void FrameProfiler::AddFrame(float deltaTime, std::chrono::steady_clock::time_point now)
{
	m_frames.push_back({ deltaTime, now });

	m_sumDeltaTimeMs += deltaTime;
	m_sumRecipDeltaTimeMs += 1.0f / deltaTime;

	// clear old frames
	while (!m_frames.empty() && m_frames.front().timestamp <= now - std::chrono::duration<float>(m_profileWindowSecs))
	{
		m_sumDeltaTimeMs -= m_frames.front().deltaTime;
		m_sumRecipDeltaTimeMs -= 1.0f / m_frames.front().deltaTime;
		m_frames.pop_front();
	}
}

float FrameProfiler::GetAverageDelta() const
{
	float meanTime = 0;
	for (const FrameInfo& frame : m_frames)
	{
		meanTime += frame.deltaTime;
	}
	meanTime /= m_frames.size();
	return meanTime;
}

float FrameProfiler::GetAverageFPS() const
{
	if (m_frames.empty()) return ZEROF;
	return 1000.0f * m_sumRecipDeltaTimeMs / m_frames.size();
}

float FrameProfiler::GetHarmonicMeanFPS() const
{
	if (m_frames.empty()) return ZEROF;

	return 1000.0f *  m_frames.size()/ m_sumDeltaTimeMs;
}

float FrameProfiler::GetAvgLowOnePercent() const
{
	if (m_frames.empty()) return ZEROF;

	// 1% largest frames, at least 1 element
	size_t x = std::max<size_t>(1, m_frames.size() / 100);

	// copy to vector
	std::vector<FrameInfo> vec(m_frames.begin(), m_frames.end());

	// first x of sorted gets put at the start of the vector
	std::partial_sort(vec.begin(), vec.begin() + x, vec.end(),
		// tell the partial sort how to order the data
		[](const FrameInfo& a, const FrameInfo& b) {
			return a.deltaTime > b.deltaTime; // descending
		});

	float sumDelta = 0.0f;
	for (size_t i = 0; i < x; ++i)
		sumDelta += vec[i].deltaTime;

	float meanDelta = sumDelta / x;
	return 1000.0f / meanDelta; // FPS = 1000 / ms

}

float FrameProfiler::GetMinFPS() const
{
	if (m_frames.empty()) return ZEROF;

	return std::max_element(m_frames.begin(), m_frames.end(),
		// i need to compare detaTime, not the frameinfo class, do define how it finds "Max"
		[](const FrameInfo& a, const FrameInfo& b) {
			return a.deltaTime < b.deltaTime; // ascending
		})->deltaTime;
}
