#pragma once
class Puzzle;
class PuzzleState
{
public:
	virtual void OnUpdate() = 0;
	virtual void IncrementTimer() = 0;
	PuzzleState(Puzzle* puzzle);

protected:
	Puzzle* m_attachedPuzzle;
};

