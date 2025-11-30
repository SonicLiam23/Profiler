#pragma once
class SDL_Texture;// class SDL_Renderer;
class Image
{
public:
	Image(const char* Path);
	const char* GetPath();
	SDL_Texture* GetTexture();
private:
	const char* m_Path;
	SDL_Texture* m_Texture;
};

