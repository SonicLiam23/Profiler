#include "InputManager.h"
#include "SDL_events.h"
#include <cctype>
#include "Engine.h"
#include "Cursor.h"
#include "Rect.h"

InputManager* InputManager::s_Instance = nullptr;

InputManager::InputManager() : NewScanCode(SDL_SCANCODE_UNKNOWN)
{
	for (int i = 0; i < SDL_NUM_SCANCODES; ++i)
	{
		m_PressedKeys[i] = NONE;
	}
}


InputManager* InputManager::Get()
{
	return s_Instance != nullptr ? s_Instance : s_Instance = new InputManager();
}

void InputManager::Update()
{
	SDL_Event Event;
	for (int i = 0; i < SDL_NUM_SCANCODES; ++i)
	{
		if (m_PressedKeys[i] == DOWN)
		{
			m_PressedKeys[i] = HELD;
		}
		else if (m_PressedKeys[i] == UP)
		{
			m_PressedKeys[i] = NONE;
		}
	}

    while (SDL_PollEvent(&Event))
    {
        SDL_Scancode NewScanCode = SDL_GetScancodeFromKey(Event.key.keysym.sym);

        // Keyboard inputs
        if (Event.type == SDL_KEYDOWN)
        {
            if (m_PressedKeys[NewScanCode] == NONE)
            {
                m_PressedKeys[NewScanCode] = DOWN;
            }
        }
        else if (Event.type == SDL_KEYUP)
        {
            m_PressedKeys[NewScanCode] = UP;
        }

        // Mouse clicks
        else if (Event.type == SDL_MOUSEBUTTONDOWN)
        {
            if (Event.button.button == SDL_BUTTON_LEFT)
            {
                int x = Event.button.x;
                int y = Event.button.y;

                Rect clickRect = { x, y, 3, 3 };
                Cursor::DoClick(&clickRect);
            }
        }

        // quit
        else if (Event.type == SDL_QUIT)
        {
            Engine::Get()->Uninit();
        }
    }
}

bool InputManager::GetKeyUp(char Key) const
{
	return m_PressedKeys[Get()->GetScancodeFromChar(Key)] == UP;
}

bool InputManager::GetKeyDown(char Key) const 
{
	return m_PressedKeys[Get()->GetScancodeFromChar(Key)] == DOWN;
}

bool InputManager::GetKeyHeld(char Key) const
{
	return m_PressedKeys[Get()->GetScancodeFromChar(Key)] == HELD;
}

SDL_Scancode InputManager::GetScancodeFromChar(char key)
{
	key = static_cast<char>(std::tolower(key));
	SDL_KeyCode keycode = static_cast<SDL_KeyCode>(key);
	return SDL_GetScancodeFromKey(keycode);
}

