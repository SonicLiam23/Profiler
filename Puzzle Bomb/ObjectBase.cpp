#include "ObjectBase.h"
#include "Renderer.h"


ObjectBase::ObjectBase(Rect pos) : rect(pos), CollisionEnabled(true), CameraOffset(0)
{

}

Rect* ObjectBase::GetPosition()
{
	return &rect;
}
