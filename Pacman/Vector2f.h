#ifndef VECTOR2F_H
#define VECTOR2F_H

#include <math.h> 

class Vector2f
{
public:
	Vector2f()
	{
		myX = 0.f;
		myY = 0.f;
	}

	Vector2f(float anX, float anY)
	{
		myX = anX;
		myY = anY;
	}

	const Vector2f Vector2f::operator-(const Vector2f &other) const 
	{
		Vector2f v(myX - other.myX, myY - other.myY);
		return v;
	}

	const Vector2f Vector2f::operator+(const Vector2f &other) const 
	{
		Vector2f v(myX + other.myX, myY + other.myY);
		return v;
	}

	
	const Vector2f Vector2f::operator*(const Vector2f& other) const 
	{
		Vector2f v(myX*other.myX, myY*other.myY);
		return v;
	}

	Vector2f& Vector2f::operator+=(const Vector2f &other) 
	{
		myX = myX + other.myX;
		myY = myY + other.myY;

		return *this;
	}

	Vector2f& Vector2f::operator*=(const float aFloat) 
	{
		myX *= aFloat;
		myY *= aFloat;

		return *this;
	}

	Vector2f& Vector2f::operator/=(const float aFloat) 
	{
		myX /= aFloat;
		myY /= aFloat;

		return *this;
	}

	const Vector2f Vector2f::operator*(const float aValue) const 
	{
		Vector2f v(myX * aValue, myY * aValue);
		return v;
	}

	float Vector2f::Length() const
	{
		return sqrt(myX*myX + myY*myY);
	}

	void Vector2f::Normalize()
	{
		float length = Length();

		if (length > 0.f)
			*this /= length;
	}

	float myX;
	float myY;
};

#endif // VECTOR2F_H