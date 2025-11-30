#include "ImageObject.h"
#include "Renderer.h"
#include "Image.h"

ImageObject::ImageObject() : m_img(nullptr)
{
}

void ImageObject::Render()
{
	if (m_img != nullptr)
	{
		Renderer::Get()->DrawObject(m_img->GetTexture(), &rect);
	}
}
void ImageObject::SetImage(const char* Path)
{
	m_img = Renderer::Get()->AddImage(Path);
}

const char* ImageObject::GetName()
{
    return nullptr;
}
