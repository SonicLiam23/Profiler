#include "SDLClasses.h"
#include "Globals.h"

#include "SDL.h"

SDL_Window* SDLClasses::s_Window = nullptr;
SDL_Renderer* SDLClasses::s_Renderer = nullptr;

SDL_Window* SDLClasses::GetWindow()
{
    return s_Window != nullptr ? s_Window : s_Window = SDL_CreateWindow("GameWindow", 2, 2, Globals::SCREEN_WIDTH, Globals::SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
}

SDL_Renderer* SDLClasses::GetRenderer()
{
    return s_Renderer != nullptr ? s_Renderer : s_Renderer = SDL_CreateRenderer(SDLClasses::GetWindow(), -1, 0);
}

void SDLClasses::DeleteAll()
{
    SDL_DestroyWindow(SDLClasses::GetWindow());
    SDL_DestroyRenderer(SDLClasses::GetRenderer());
}