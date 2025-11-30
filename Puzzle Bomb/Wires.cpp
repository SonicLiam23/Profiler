#include "Wires.h"
#include <algorithm>
#include <vector>
#include <random>
#include "Background.h"
#include "Engine.h"
#include "TextObject.h"
#include "Wire.h"


Wires::Wires() : Puzzle(100)
{
    SetImage("wireBG.bmp");

 
    std::vector<WireColour> colours = { RED, BLUE, YELLOW };

    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(colours.begin(), colours.end(), g);

    std::uniform_int_distribution<> dist(0, colours.size() - 1);
    int correctIndex = dist(g);

    m_wireSize *= PIX_MULT;
    for (int i = 0; i < 3; ++i)
    {
        m_wires[i] = new Wire(colours[i]);
        m_wireRects[i].x *= PIX_MULT;
        m_wireRects[i].y *= PIX_MULT;
        m_wireRects[i].x += Background::currentPuzzleSlot.x;
        m_wireRects[i].y += Background::currentPuzzleSlot.y;
        m_wires[i]->rect = { m_wireRects[i].x, m_wireRects[i].y, m_wireSize.x, m_wireSize.y };

        m_wires[i]->wirePuzzle = this;

        Engine::Get()->AddObject(m_wires[i]);
    }

    m_wires[correctIndex]->correctWire = true;

    m_instruction = new TextObject("Kenney Pixel.ttf", 200);
    m_textPosition *= PIX_MULT;
    m_textPosition.x += Background::currentPuzzleSlot.x;
    m_textPosition.y += Background::currentPuzzleSlot.y;
    
    m_instruction->rect.x = m_textPosition.x + 24;
    m_instruction->rect.y = m_textPosition.y;

    switch (m_wires[correctIndex]->Colour)
    {
    case RED:    
        m_instruction->SetText("Cut Red"); 
        break;
    case BLUE:   
        m_instruction->SetText("Cut Blue"); 
        break;
    case YELLOW: 
        m_instruction->SetText("Cut Yellow"); 
        break;
    };
    Engine::Get()->AddObject(m_instruction);

}

void Wires::CompletePuzzle(bool createNew)
{
    Engine::Get()->DeleteObject(m_instruction);
    for (int i = 0; i < 3; ++i)
    {
        Engine::Get()->DeleteObject(m_wires[i]);
    }
    Puzzle::CompletePuzzle(createNew);
}

void Wires::SlotPuzzle(Vector2 puzzlePos, Vector2 timerPos, bool isMainSlot)
{
    Puzzle::SlotPuzzle(puzzlePos, timerPos, isMainSlot);

    if (isMainSlot)
    {
        for (int i = 0; i < 3; ++i)
        {
            m_wires[i]->rect.x = m_wireRects[i].x;
            m_wires[i]->rect.y = m_wireRects[i].y;
        }
        m_instruction->rect.x = m_textPosition.x; m_instruction->rect.y = m_textPosition.y;
    }
    else
    {
        for (int i = 0; i < 3; ++i)
        {
            m_wires[i]->rect.x = puzzlePos.x;
            m_wires[i]->rect.y = puzzlePos.y;
        }
        m_instruction->rect.x = puzzlePos.x; m_instruction->rect.y = puzzlePos.y;
    }

}




