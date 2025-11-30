#pragma once
#include <chrono>
#include "Vector2.h"
#include "ImageObject.h"
struct TimerDisplay; class PuzzleState;


enum PuzzleStates
{
	SLOTTED,
	ACTIVE
};

enum PuzzleType
{
	WIRES,
	EVENODD,
	PUZZLE_TYPE_COUNT
};

class Puzzle
	: public ImageObject
{
private:
	
	
	
protected:
	
	TimerDisplay* m_timer;
	PuzzleState* m_currentState;
	PuzzleState* m_allStates[2];
	

public:
	Puzzle(int puzzleTime);
	~Puzzle() {/* delete all */}
	virtual void Update() override;
	const char* GetName() override;
	virtual void SlotPuzzle(Vector2 puzzlePos, Vector2 timerPos, bool isMainSlot = false);
	virtual void CompletePuzzle(bool createNew = true);

	static Puzzle* CreateNewPuzzle();
	TimerDisplay* GetTimer() { return m_timer; }
	int SecondsLeft;
	void SetState(PuzzleStates state);
};

