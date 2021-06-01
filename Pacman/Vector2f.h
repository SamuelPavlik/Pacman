#ifndef VECTOR2F_H
#define VECTOR2F_H

#include <math.h> 

class Vector2f
{
public:
	Vector2f() noexcept
	{
		x = 0.f;
		y = 0.f;
	}

	Vector2f(float anX, float anY) noexcept
	{
		x = anX;
		y = anY;
	}

	Vector2f Vector2f::operator-(const Vector2f &other) const noexcept
	{
		Vector2f v(x - other.x, y - other.y);
		return v;
	}

	Vector2f Vector2f::operator+(const Vector2f &other) const noexcept
	{
		Vector2f v(x + other.x, y + other.y);
		return v;
	}

	
	Vector2f Vector2f::operator*(const Vector2f& other) const noexcept
	{
		Vector2f v(x*other.x, y*other.y);
		return v;
	}

	Vector2f& Vector2f::operator+=(const Vector2f &other) noexcept
	{
		x += other.x;
		y += other.y;

		return *this;
	}

	bool Vector2f::operator==(const Vector2f& other) const noexcept
	{
		return x == other.x && y == other.y;
	}

	bool Vector2f::operator!=(const Vector2f& other) const noexcept
	{
		return !(*this == other);
	}

	Vector2f& Vector2f::operator*=(const float aFloat) noexcept
	{
		x *= aFloat;
		y *= aFloat;

		return *this;
	}

	Vector2f& Vector2f::operator/=(const float aFloat) noexcept
	{
		x /= aFloat;
		y /= aFloat;

		return *this;
	}

	Vector2f Vector2f::operator*(const float aValue) const noexcept
	{
		Vector2f v(x * aValue, y * aValue);
		return v;
	}

	Vector2f Vector2f::operator/(const float aValue) const noexcept
	{
		Vector2f v(x / aValue, y / aValue);
		return v;
	}

	float Vector2f::Length() const noexcept
	{
		return sqrt(x * x + y * y);
	}

	void Vector2f::Normalize() noexcept
	{
		float length = Length();

		if (length > 0.f)
			*this /= length;
	}

	float x;
	float y;
};

#endif // VECTOR2F_H