#pragma once
#include "Rect.h"
#include "ManagedMemory.h"
class Image; class ImageObject; class TextObject;
/*
* Base for the objects
* each object will need an update, render, start, end and a function to return its own name
* each class inherited from this will be its own object, like unity, each unique object will have its own class
* 
*/
class ObjectBase : public ManagedMemory
{
public:
	virtual void Update() {}
	virtual void Render() = 0;
	virtual const char* GetName() = 0;
	virtual void OnClick() {}
	bool CollisionEnabled;
	Rect* GetPosition();
	Rect rect;

protected:
	int CameraOffset;
	ObjectBase(Rect pos = { 0, 0, 320, 320 });
	friend class ImageObject; friend class TextObject;

};