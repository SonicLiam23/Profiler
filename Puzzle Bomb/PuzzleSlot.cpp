#include "PuzzleSlot.h"
#include "Puzzle.h"
#include "TextObject.h"
#include "TimerDisplay.h"
#include "Globals.h"
#include "Background.h"

Puzzle* PuzzleSlot::m_currentPuzzle = nullptr;
PuzzleSlot* PuzzleSlot::AllSlots[4];

PuzzleSlot::PuzzleSlot(Vector2 timerPos) : m_timerPosition(timerPos), StoredPuzzle(nullptr)
{
	//SetImage("box.bmp");
}

const char* PuzzleSlot::GetName()
{
	return "PuzzleSlot";
}

void PuzzleSlot::OnClick()
{
	if (m_currentPuzzle != nullptr)
	{

		if (m_currentPuzzle == StoredPuzzle)
		{
			return;
		}
		// ... move the current one offscreen, and put its timer on the slot
		m_currentPuzzle->SlotPuzzle({ Globals::SCREEN_WIDTH + 10,  -Globals::HALF_SCREEN_HEIGHT - 10 }, {rect.x, rect.y});

	}
	
	// if there is a puzzle stored ...
	if (StoredPuzzle != nullptr)
	{
		// ... swap the current puzzle with the one stored ...
		Puzzle* oldStored = StoredPuzzle;
		StoredPuzzle = m_currentPuzzle;
		m_currentPuzzle = oldStored;
		m_currentPuzzle->SlotPuzzle({ Background::currentPuzzleSlot.x, Background::currentPuzzleSlot.y }, { Background::currentPuzzleTimer.x, Background::currentPuzzleTimer.y }, true);
	}
	// ... if not
	else
	{
		StoredPuzzle = m_currentPuzzle;
		Puzzle::CreateNewPuzzle();
	}
	
}

void PuzzleSlot::AdjustTimerOnSlottedPuzzle(int adjustment)
{
	if (StoredPuzzle != nullptr)
	{
		StoredPuzzle->SecondsLeft += adjustment;
		StoredPuzzle->GetTimer()->SetText(StoredPuzzle->SecondsLeft);
	}
}


void PuzzleSlot::SetCurrentPuzzle(Puzzle* puzzle)
{
	if (puzzle != nullptr)
	{
		m_currentPuzzle = puzzle;
	}
	else
	{
		m_currentPuzzle->CompletePuzzle(false);
		m_currentPuzzle = nullptr;
	}
}


