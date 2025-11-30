#include "SlottedState.h"
#include "TimerDisplay.h"
#include "Puzzle.h"
#include "TextObject.h"

void SlottedState::OnUpdate()
{
}

void SlottedState::IncrementTimer()
{
	m_attachedPuzzle->GetTimer()->SetText(++m_attachedPuzzle->SecondsLeft);
}


