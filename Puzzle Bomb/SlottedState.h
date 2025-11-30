#pragma once
#include "PuzzleState.h"
class TimerDisplay; class Puzzle;
class SlottedState :
    public PuzzleState
{
    // Inherited via PuzzleState
    void OnUpdate() override;
    void IncrementTimer() override;

public:
    SlottedState(Puzzle* puzzle) : PuzzleState(puzzle){}
};

