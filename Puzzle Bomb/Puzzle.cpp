#include "Puzzle.h"
#include "Engine.h"
#include "TimerDisplay.h"
#include "Background.h"
#include <string>
#include "PuzzleState.h"
#include "ActiveState.h"
#include "SlottedState.h"
#include "PuzzleSlot.h"
#include <random>
#include "wires.h"
#include "EvenOdd.h"


void Puzzle::CompletePuzzle(bool createNew)
{
    if (createNew)
    {
        for (PuzzleSlot* slot : PuzzleSlot::AllSlots)
        {
            slot->AdjustTimerOnSlottedPuzzle(1);
        }
    }

    delete m_allStates[ACTIVE];
    delete m_allStates[SLOTTED];

    Engine::Get()->DeleteObject(m_timer);
    m_timer = nullptr;
    if (createNew) CreateNewPuzzle();
    Engine::Get()->DeleteObject(this);
}

Puzzle::Puzzle(int puzzleTime) : SecondsLeft(puzzleTime)
{
    Engine::Get()->AddObject(this);

   m_timer = new TimerDisplay();
   m_timer->SetText(SecondsLeft);

   // when creating a puzzle, it will always be the one in the middle.
   m_timer->rect.x = Background::currentPuzzleTimer.x;
   m_timer->rect.y = Background::currentPuzzleTimer.y;
   rect = Background::currentPuzzleSlot;

   Engine::Get()->AddObject(m_timer);

   m_allStates[ACTIVE] = new ActiveState(this);
   m_allStates[SLOTTED] = new SlottedState(this);

   m_currentState = m_allStates[ACTIVE];

   PuzzleSlot::SetCurrentPuzzle(this);
}

void Puzzle::Update()
{
    m_currentState->OnUpdate();
}

const char* Puzzle::GetName()
{
    return "Puzzle";
}

void Puzzle::SlotPuzzle(Vector2 puzzlePos, Vector2 timerPos, bool isMainSlot)
{
    rect.x = puzzlePos.x; rect.y = puzzlePos.y;

    m_timer->rect.x = timerPos.x; m_timer->rect.y = timerPos.y;

    if (!isMainSlot)
    {
        m_currentState = m_allStates[SLOTTED];
    }
    else
    {
        m_currentState = m_allStates[ACTIVE];
    }
}

Puzzle* Puzzle::CreateNewPuzzle()
{
    // when creating a new one, it will always be the one put in the middle

    PuzzleType nextPuzzle = (PuzzleType)(rand() % PUZZLE_TYPE_COUNT);
    Puzzle* newPuzzle = nullptr;
    switch (nextPuzzle)
    {
    case WIRES:
        newPuzzle = new Wires();
        break;
    case EVENODD:
        newPuzzle = new EvenOdd();
        break;
 
    }

    return newPuzzle;
}

void Puzzle::SetState(PuzzleStates state)
{
    m_currentState = m_allStates[state];
}
