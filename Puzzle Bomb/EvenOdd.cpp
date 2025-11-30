#include "EvenOdd.h"
#include "InputManager.h"
#include "TimerDisplay.h"
#include "Background.h"
#include "Engine.h"

EvenOdd::EvenOdd() : Puzzle(10)
{
    m_instruction = new TextObject("Kenney Pixel.ttf", 100);
    m_textPosition *= PIX_MULT;
    m_textPosition.x += Background::currentPuzzleSlot.x;
    m_textPosition.y += Background::currentPuzzleSlot.y;

    m_instruction->SetText("Is the timer even or odd? (e/o)");

    m_instruction->rect = { m_textPosition.x, m_textPosition.y, m_instruction->rect.w, m_instruction->rect.h };
    Engine::Get()->AddObject(m_instruction);

}

void EvenOdd::Update()
{
    if (m_currentState != m_allStates[SLOTTED])
    {
        Puzzle::Update();

        if (InputManager::Get()->GetKeyDown('o') || InputManager::Get()->GetKeyDown('e'))
        {
            bool isEKey = InputManager::Get()->GetKeyDown('e');
            bool isEven = (SecondsLeft % 2 == 0);

            if ((isEKey && isEven) || (!isEKey && !isEven))
            {
                CompletePuzzle();
            }
            else
            {
                SecondsLeft = 0;
            }
        }
    }
}

void EvenOdd::SlotPuzzle(Vector2 puzzlePos, Vector2 timerPos, bool isMainSlot)
{
	Puzzle::SlotPuzzle(puzzlePos, timerPos, isMainSlot);
}

void EvenOdd::CompletePuzzle(bool createNew)
{
    if (m_instruction)
    {
        Engine::Get()->DeleteObject(m_instruction);
        Puzzle::CompletePuzzle(createNew);
        m_instruction = nullptr;
    }
    else
    {
        int i = 7;
    }
}
