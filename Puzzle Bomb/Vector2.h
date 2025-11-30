#pragma once
struct Vector2
{
	int x, y;	

	constexpr Vector2() : x(0), y(0) {}
	constexpr Vector2(int px, int py) : x(px), y(py) {}

    constexpr Vector2 operator+(const Vector2& rhs) const 
	{
        return {x + rhs.x, y + rhs.y};
    }
     
    constexpr Vector2 operator-(const Vector2& rhs) const 
    {
        return {x - rhs.x, y - rhs.y};
    }

    constexpr Vector2 operator*(const Vector2& rhs) const 
    {
        return {x * rhs.x, y * rhs.y};
    }

    constexpr Vector2 operator/(const Vector2& rhs) const 
    {
        return {x / rhs.x, y / rhs.y};
    }

    constexpr Vector2 operator*(int scalar) const 
    {
        return {x * scalar, y * scalar};
    }

    constexpr Vector2 operator/(int scalar) const 
    {
        return {x / scalar, y / scalar};
    }

    Vector2& operator+=(const Vector2& rhs) 
    {
        x += rhs.x; y += rhs.y;
        return *this;
    }

    Vector2& operator-=(const Vector2& rhs) 
    {
        x -= rhs.x; y -= rhs.y;
        return *this;
    }

    Vector2& operator*=(int scalar) 
    {
        x *= scalar; y *= scalar;
        return *this;
    }

    Vector2& operator/=(int scalar) 
    {
        x /= scalar; y /= scalar;
        return *this;
    }

    constexpr bool operator==(const Vector2& rhs) const 
    {
        return x == rhs.x && y == rhs.y;
    }

    constexpr bool operator!=(const Vector2& rhs) const 
    {
        return !(*this == rhs);
    }
};

