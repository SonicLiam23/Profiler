#pragma once
#include "Puzzle.h"
#include "Rect.h"
#include "Background.h"

class TextObject; class Wire;

class Wires :
    public Puzzle
{
public:
    Wires();
    void CompletePuzzle(bool createNew = true) override;
    void SlotPuzzle(Vector2 puzzlePos, Vector2 timerPos, bool isMainSlot = false) override;

private:
    Vector2 m_wireRects[3] =
    {
        {6, 16},
        {6, 20},
        {6, 24}
    };
    Vector2 m_wireSize = { 35, 2 };
    Vector2 m_textPosition = {3, 3};
    
    Wire* m_wires[3];

    TextObject* m_instruction;

};



