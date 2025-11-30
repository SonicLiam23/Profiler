#include "Renderer.h"
#include "Globals.h"
#include "SDLCLasses.h"
#include "Image.h"
#include "Text.h"
#include "ObjectBase.h"

#include "SDL_ttf.h"
#include "SDL.h"

Renderer* Renderer::s_instance = nullptr;

Renderer::Renderer()
{
    // creates a renderer if it doesnt exist
    SDLClasses::GetRenderer();
    SDLClasses::GetWindow();

    CameraCentre = nullptr;
}

Image* Renderer::AddImage(const char* Path)
{
	m_StoredImages.insert({ Path, new Image(Path) });
    return m_StoredImages.at(Path);
}

TTF_Font* Renderer::AddFont(const char* path, int size)
{
    std::pair<const char*, int> key = { path, size };
    m_StoredFonts.insert({ key, TTF_OpenFont(path, size) });
    return m_StoredFonts.at(key);
}

void Renderer::DrawObject(SDL_Texture* img, Rect* rect, int YOffset)
{
    SDL_Rect temp = rect->TryParse<SDL_Rect>();
    float OffsetX = rect->w / 2;
    float OffsetY = YOffset + (rect->h / 2);
    if (CameraCentre != nullptr)
    {
        temp.x += (Globals::HALF_SCREEN_WIDTH - CameraCentre->rect.x - OffsetX);
        temp.y += (Globals::HALF_SCREEN_HEIGHT - CameraCentre->rect.y - OffsetY);
    }
   
	SDL_RenderCopy(SDLClasses::GetRenderer(), img, NULL, &temp);
}

void Renderer::SetCamera(ObjectBase* centre)
{
    CameraCentre = centre;
}

Renderer* Renderer::Get()
{
    return s_instance != nullptr ? s_instance : s_instance = new Renderer();
}
