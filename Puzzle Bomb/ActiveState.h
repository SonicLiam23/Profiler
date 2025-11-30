#pragma once
#include "PuzzleState.h"
#include <chrono>

std::chrono::seconds constexpr delay = std::chrono::seconds(1);

class ActiveState :
    public PuzzleState
{
    // Inherited via PuzzleState
    void OnUpdate() override;
    void IncrementTimer() override;
    std::chrono::steady_clock::time_point m_endTime;

public:
    ActiveState(Puzzle* puzzle);
};

