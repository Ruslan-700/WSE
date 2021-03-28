#include "rgl_vector2.h"

#include <cmath>

using namespace rgl;

vector2::vector2(float f)
{
	this->x = f;
	this->y = f;
}

vector2::vector2(float x, float y)
{
	this->x = x;
	this->y = y;
}

float vector2::dot(const vector2 &v) const
{
	return x * v.x + y * v.y;
}

float vector2::length() const
{
	return std::sqrt(x * x + y * y);
}

float vector2::length_squared() const
{
	return x * x + y * y;
}

float vector2::normalize()
{
	float len = length();

	if (len <= 0.0f)
	{
		x = 0.0f;
		y = 1.0f;
	}
	else
	{
		x /= len;
		y /= len;
	}

	return len;
}

vector2 vector2::normalized()
{
	vector2 vec = *this;
	
	vec.normalize();
	return vec;
}

vector2::operator float *()
{
	return (float *)this;
}

vector2::operator float *() const
{
	return (float *)this;
}
