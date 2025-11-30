#include "Wire.h"
#include "Wires.h"
#include <iostream>

Wire::Wire(WireColour col) : Colour(col)
{
    switch (Colour)
    {
    case BLUE:
        SetImage("blue.bmp");
        break;
    case RED:
        SetImage("red.bmp");
        break;
    case YELLOW:
        SetImage("yellow.bmp");
        break;
    }
}

const char* Wire::GetName()
{
    return "Wire";
}

void Wire::OnClick()
{
    if (correctWire)
    {
        std::cout << "Horray!!!";
        wirePuzzle->CompletePuzzle();
    }
    else
    {
        std::cout << "booooo";
        wirePuzzle->SecondsLeft = 0;
    }
}