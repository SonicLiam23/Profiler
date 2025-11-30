#pragma once
#include "ImageObject.h"

enum WireColour
{
    RED,
    BLUE,
    YELLOW
};

class Wires;
struct Wire
    : ImageObject
{
    Wires* wirePuzzle;
    Wire(WireColour col);
    WireColour Colour;
    bool correctWire = false;

    // Inherited via ImageObject
    const char* GetName() override;

    void OnClick() override;
};

