#pragma once
//	SDL classes, such as the window and renderer, are referenced in many places
//  this class will be used to keep them all in one seperate place, to avoid the need to have them in the engine or elsewhere

class SDL_Window; class SDL_Renderer;

class SDLClasses
{
public:
	static SDL_Window* GetWindow();
	static SDL_Renderer* GetRenderer();
	static void DeleteAll();


private:
	static SDL_Window* s_Window;
	static SDL_Renderer* s_Renderer;
};

