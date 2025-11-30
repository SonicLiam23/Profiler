#pragma once
#include "ObjectBase.h"
class Image;
class ImageObject
	: public ObjectBase
{
public:
	ImageObject();
	void Render() override;
	virtual const char* GetName() = 0;
	void SetImage(const char* path);

private:
	Image* m_img;
};

