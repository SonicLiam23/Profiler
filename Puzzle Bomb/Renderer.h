#pragma once
#include "Globals.h"
#include <map>
#include <utility>
struct Rect;
class SDL_Texture; class Image; class SDL_Rect; class ObjectBase; class TTF_Font;
class Text;
class Renderer
{
public:
	static Renderer* Get();
	Image* AddImage(const char* Path);
	TTF_Font* AddFont(const char* path, int size);
	void DrawObject(SDL_Texture* img, Rect* rect, int YOffset = 0);
	ObjectBase* CameraCentre;
	void SetCamera(ObjectBase* centre);


private:
	Renderer();
	//~Renderer();
	static Renderer* s_instance;
	// Key - Image Path
	// Value - Image Class
	std::map<const char*, Image*> m_StoredImages;
	std::map<std::pair<const char*, int>, TTF_Font*> m_StoredFonts;

};

