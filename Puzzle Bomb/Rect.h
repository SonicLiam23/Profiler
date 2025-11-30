#pragma once
#include "Log.h"
struct Rect
{
	int x, y, w, h;
	constexpr Rect(int px, int py, int pw, int ph) : x(px), y(py), w(pw), h(ph) {}
	constexpr Rect() : x(0), y(0), w(0), h(0) {}

	template <typename T>
	const T TryParse()
	{
		T out;
		try
		{
			out.x = x;
			out.y = y;
			out.w = w;
			out.h = h;
		}
		catch (...)
		{
			LOGTEXTM("Error converting in Rect.h");
		}
		return out;
	}

	Rect& operator*=(int scalar)
	{
		x *= scalar; y *= scalar; w *= scalar; h *= scalar;
		return *this;
	}
};
