#include "BombRush.h"
#include "Objects.h"
#include "Background.h"
#include "PuzzleSlot.h"
#include "Puzzle.h"
#include "TimerDisplay.h"
#include "SlottedState.h"
#include <iostream>
#include "Wires.h"
#include <random>
#include "Engine.h"

//#include "SDL_ttf.h"

Background* BombRush::bg = nullptr;

void BombRush::Start()
{
	srand(time(NULL));

	Engine* e = Engine::Get();
	e->Init();

	bg = new Background();
	e->AddObject(bg);

	// CORRECT PUZZLE SLOTS!!!!
	for (int i = 0; i < 4; ++i)
	{
		PuzzleSlot::AllSlots[i] = new PuzzleSlot({ bg->puzzleSlotPositions[i].x, bg->puzzleSlotPositions[i].y });
		PuzzleSlot::AllSlots[i]->rect = { bg->puzzleSlotPositions[i].x, bg->puzzleSlotPositions[i].y, bg->slotDim.x, bg->slotDim.y };
		e->AddObject(PuzzleSlot::AllSlots[i]);
	}
	// KEEP CODE ABOVE FOR REFERENCE

	Wires* puzzle = new Wires();
	// set as main puzzle
	puzzle->SlotPuzzle({ bg->currentPuzzleSlot.x, bg->currentPuzzleSlot.y }, { bg->currentPuzzleTimer.x, bg->currentPuzzleTimer.y }, true);

	PuzzleSlot::SetCurrentPuzzle(puzzle);
}


void BombRush::RestartGame()
{
	Engine::Get()->DeleteObject(bg);
	BombRush::Start();
}

void BombRush::EndGame()
{
	bg->GameOverScreen();
	for (PuzzleSlot* slot : PuzzleSlot::AllSlots)
	{
		Puzzle* puzzleInSlot = slot->StoredPuzzle;
		if (puzzleInSlot != nullptr)
		{
			puzzleInSlot->CompletePuzzle(false);
		}
		Engine::Get()->DeleteObject(slot);
	}
	PuzzleSlot::SetCurrentPuzzle(nullptr);
	
}
