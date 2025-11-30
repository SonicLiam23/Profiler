#include "Text.h"
#include "Renderer.h"
#include "SDLClasses.h"
#include "SDL_Render.h"
#include "SDL_ttf.h"
#include "Log.h"


void Text::SetFont(const char* font, int size)
{
	if (m_font == nullptr)
	{
		m_font = Renderer::Get()->AddFont(font, size);
	}
		
}

void Text::SetText(const char* text, int* w, int* h)
{
	if (m_font != nullptr)
	{
		if (m_texture != nullptr)
		{
			SDL_DestroyTexture(m_texture);										
		}
		SDL_Surface* surface = TTF_RenderText_Solid(m_font, text, { 255, 255, 255, 255 });
		m_texture = SDL_CreateTextureFromSurface(SDLClasses::GetRenderer(), surface);
		SDL_FreeSurface(surface);

		TTF_SizeUTF8(m_font, text, w, h);
	}
	else
	{
		LOGTEXTM("Couldnt change text, font is nullptr.");
	}

}

SDL_Texture* Text::GetTexture()
{
	return m_texture;
}


