#pragma once
#include "ImageObject.h"
#include "Vector2.h"
class Puzzle; struct TimerDisplay; class PuzzleState;


class PuzzleSlot :
    public ImageObject
{
private:

    Vector2 m_timerPosition;


    static Puzzle* m_currentPuzzle;

public:
    
    PuzzleSlot(Vector2 timerPos);

    // Inherited via ImageObject
    const char* GetName() override;

    void OnClick() override;

    void AdjustTimerOnSlottedPuzzle(int adjustment);

    Puzzle* StoredPuzzle;

    static void SetCurrentPuzzle(Puzzle* puzzle);

    static PuzzleSlot* AllSlots[4]; 
};