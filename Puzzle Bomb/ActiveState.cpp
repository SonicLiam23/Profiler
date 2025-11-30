#include "ActiveState.h"
#include "Engine.h"
#include "Puzzle.h"
#include "TimerDisplay.h"
#include "BombRush.h"

ActiveState::ActiveState(Puzzle* puzzle) : PuzzleState(puzzle) 
{
    m_endTime = std::chrono::steady_clock::now() + delay;
}

void ActiveState::OnUpdate()
{
    if (std::chrono::steady_clock::now() >= m_endTime)
    {
        m_attachedPuzzle->SecondsLeft--;
        m_attachedPuzzle->GetTimer()->SetText(m_attachedPuzzle->SecondsLeft);

        m_endTime = std::chrono::steady_clock::now() + delay;

        if (m_attachedPuzzle->SecondsLeft <= 0)
        {
            BombRush::EndGame();
        }
    }
}

void ActiveState::IncrementTimer()
{
}
