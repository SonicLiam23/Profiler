#pragma once
#include "SDL_scancode.h"
#include "SDL_keycode.h"

/// Defines special characters that may be more difficult to express such as Escape (\x1B), tab (\t) and return (\r)
namespace SpecialKey
{
    constexpr char ESC = SDLK_ESCAPE;
    constexpr char TAB = SDLK_TAB;
    constexpr char BKSPC = SDLK_BACKSPACE;
    constexpr char ENT = SDLK_RETURN;
    constexpr char SPC = SDLK_SPACE;
    constexpr char DEL = SDLK_DELETE;
	constexpr char UP = SDLK_UP;
	constexpr char DOWN = SDLK_DOWN;
	constexpr char LEFT = SDLK_LEFT;
	constexpr char RIGHT = SDLK_RIGHT;
};



class InputManager
{
public:
	static InputManager* Get();
	void Update();
	
	bool GetKeyUp(char Key) const;
	bool GetKeyDown(char Key) const;
	bool GetKeyHeld(char Key) const;

	
	

private:	
	enum KEYPOS
	{
		NONE,
		UP,
		DOWN,
		HELD
	};

	InputManager();
	static InputManager* s_Instance;
	SDL_Scancode NewScanCode;
	KEYPOS m_PressedKeys[SDL_NUM_SCANCODES];
    SDL_Scancode GetScancodeFromChar(char key);

};	

