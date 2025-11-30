#include "Image.h"
#include "SDLClasses.h"
#include "SDL.h"
#include <iostream>

Image::Image(const char* Path) : m_Path(Path)
{
	SDL_Surface* Surface = SDL_LoadBMP(Path);

	if (Surface == nullptr)
	{
		std::cout << "Surface is nullptr path is: " << Path << ". Is this correct?";
		system("pause");
	}

	SDL_SetColorKey(Surface, SDL_TRUE, 0xFF00FF);
	m_Texture = SDL_CreateTextureFromSurface(SDLClasses::GetRenderer(), Surface);
	if (m_Texture == NULL)
	{
		std::cout << SDL_GetError() << std::endl;
		std::cout << "Texture is NULL, idk why lmao figure it out yourself.";
	}
	SDL_FreeSurface(Surface);
}

const char* Image::GetPath()
{
	return m_Path;
}

SDL_Texture* Image::GetTexture()
{
	return m_Texture;
}
