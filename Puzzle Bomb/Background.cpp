#include "Background.h"
#include "Globals.h"
#include "InputManager.h"
#include "BombRush.h"

bool Background::m_initialized = false;

/// <summary>
/// slot 1: 4 across 14 down
/// slot 2: 4 across 26 down

/// slot 3 : 68 across 14 down
/// slot 4 : 68 across 26 down
/// </summary>
Vector2 Background::puzzleSlotPositions[4] = {
    {3, 13},
    {3, 25},
    {67, 13},
    {67, 25}
};
Vector2 Background::slotDim = { 9, 7 };

// stores the main slot
Rect Background::currentPuzzleSlot = { 16, 12, 47, 29 };
Vector2 Background::currentPuzzleTimer = { 36 , 3 };

Background::Background() : m_gameOver(false)
{
	SetImage("bg.bmp");
	rect.w = Globals::SCREEN_WIDTH;
	rect.h = Globals::SCREEN_HEIGHT;

	if (!m_initialized)
	{
		for (int i = 0; i < 4; ++i)
		{
			puzzleSlotPositions[i] *= PIX_MULT;
		}
		currentPuzzleSlot.x *= PIX_MULT;
		currentPuzzleSlot.y *= PIX_MULT;
		currentPuzzleSlot.w *= PIX_MULT;
		currentPuzzleSlot.h *= PIX_MULT;
		currentPuzzleTimer.x *= PIX_MULT;
		currentPuzzleTimer.y *= PIX_MULT;

		slotDim *= PIX_MULT;
	}

	m_initialized = true;
}

const char* Background::GetName()
{
	return "Background";
}

void Background::GameOverScreen()
{
	SetImage("explode.bmp");
	m_gameOver = true;
}

void Background::Update()
{
	if (m_gameOver && InputManager::Get()->GetKeyDown(SpecialKey::SPC))
	{
		BombRush::RestartGame();
	}
}
