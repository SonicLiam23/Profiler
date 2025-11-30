#pragma once
#include "ImageObject.h"
#include "Vector2.h"
#include "Rect.h"

// Window size is 1920x1080, bg image size is 80x45, meaninig each "pixel" on the background is 24x24 screen pixels
constexpr int PIX_MULT = 24;

class Background :
    public ImageObject
{

private:
    bool m_gameOver;
    static bool m_initialized;
public:
    Background();
    /// <summary>
    /// slot 1: 4 across 14 down
    /// slot 2: 4 across 26 down

    /// slot 3 : 68 across 14 down
    /// slot 4 : 68 across 26 down
    /// </summary>
    static Vector2 puzzleSlotPositions[4];
    static Vector2 slotDim;

    // stores the main slot
    static Rect currentPuzzleSlot;
    static Vector2 currentPuzzleTimer;

    // Inherited via ImageObject
    const char* GetName() override;

    void GameOverScreen();
    void Update() override;
};

