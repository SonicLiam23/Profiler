#pragma once
/// <summary>
/// Text works very similar to image, however, the difference is:
/// Images may be used multiple times, so I store the reference to the Image class in renderer, so only 1 instance per filepath exists
/// however, with text, each text may be different its the FONT that will be the same. So i store the font in Renderer, and then each TextObject will have one Text instance (this), that contains the SDL stuff
/// </summary>
class SDL_Texture; class TTF_Font;
class Text
{
public:
	void SetFont(const char* font, int size);
	// w and h are the width and height of the object, and are ajusted to fit the fontsize and text
	void SetText(const char* text, int* w, int* h);
	SDL_Texture* GetTexture();

private:
	SDL_Texture* m_texture;
	TTF_Font* m_font;
};

