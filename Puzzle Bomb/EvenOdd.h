#pragma once
#include "Puzzle.h"
#include "Vector2.h";

class TextObject; 

class EvenOdd :
    public Puzzle
{
public:
    EvenOdd();
    void Update() override;
    void SlotPuzzle(Vector2 puzzlePos, Vector2 timerPos, bool isMainSlot = false) override;
    void CompletePuzzle(bool createNew = true) override;

private:
    TextObject* m_instruction;
    Vector2 m_textPosition = { 2, 2 };
};

