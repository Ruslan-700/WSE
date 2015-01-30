#include "rgl_vector4.h"

#include "rgl_matrix.h"
#include <cmath>

using namespace rgl;

const vector4 vector4::zero = vector4(0.0f);
const vector4 vector4::one = vector4(1.0f);

vector4::vector4(float f, float w)
{
	this->x = f;
	this->y = f;
	this->z = f;
	this->w = w;
}

vector4::vector4(float x, float y, float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

vector4::vector4(float x, float y, float z, float w)
{
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
}

void vector4::rotate_x(float f)
{
	float cos = std::cos(f);
	float sin = std::sin(f);
	float y_old = this->y;
	float z_old = this->z;

	this->y = y_old * cos - z_old * sin;
	this->z = y_old * sin + z_old * cos;
}

void vector4::rotate_y(float f)
{
	float cos = std::cos(f);
	float sin = std::sin(f);
	float x_old = this->x;
	float z_old = this->z;

	this->x = z_old * sin + x_old * cos;
	this->z = z_old * cos - x_old * sin;
}

void vector4::rotate_z(float f)
{
	float cos = std::cos(f);
	float sin = std::sin(f);
	float x_old = this->x;
	float y_old = this->y;

	this->x = x_old * cos - y_old * sin;
	this->y = x_old * sin + y_old * cos;
}

float vector4::length() const
{
	return std::sqrt(this->x * this->x + this->y * this->y + this->z * this->z);
}

float vector4::length_squared() const
{
	return this->x * this->x + this->y * this->y + this->z * this->z;
}

void vector4::normalize()
{
	float len = this->length();

	if (len < 1e-10)
	{
		this->x = 0.0f;
		this->y = 1.0f;
		this->z = 0.0f;
		this->w = 0.0f;
	}
	else
	{
		float div = 1.0f / len;
		this->x *= div;
		this->y *= div;
		this->z *= div;
	}
}

void vector4::cross(const vector4 &v1, const vector4 &v2)
{
	this->x = v1.y * v2.z - v1.z * v2.y;
	this->y = v1.z * v2.x - v1.x * v2.z;
	this->z = v1.x * v2.y - v1.y * v2.x;
}

float vector4::dot(const vector4 &v) const
{
	return this->x * v.x + this->y * v.y + this->z * v.z;
}

bool vector4::is_unit() const
{
	float len = length();
	return len > 0.99f && len < 1.01f;
}

void vector4::transform(const vector4 &v, const matrix &m)
{
	this->x = v.x * m.rot.s.x + v.y * m.rot.f.x + v.z * m.rot.u.x + m.o.x;
	this->y = v.x * m.rot.s.y + v.y * m.rot.f.y + v.z * m.rot.u.y + m.o.y;
	this->z = v.x * m.rot.s.z + v.y * m.rot.f.z + v.z * m.rot.u.z + m.o.z;
}

vector2 vector4::vec2() const
{
	return rgl::vector2(x, y);
}

bool vector4::operator ==(const vector4 &v) const
{
	return this->x == v.x && this->y == v.y && this->z == v.z;
}

bool vector4::operator !=(const vector4 &v) const
{
	return !(*this == v);
}

void vector4::operator +=(const vector4 &v)
{
	this->x += v.x;
	this->y += v.y;
	this->z += v.z;
}

void vector4::operator +=(float f)
{
	this->x += f;
	this->y += f;
	this->z += f;
}

void vector4::operator -=(const vector4 &v)
{
	this->x -= v.x;
	this->y -= v.y;
	this->z -= v.z;
}

void vector4::operator -=(float f)
{
	this->x -= f;
	this->y -= f;
	this->z -= f;
}

void vector4::operator *=(const vector4 &v)
{
	this->x *= v.x;
	this->y *= v.y;
	this->z *= v.z;
}

void vector4::operator *=(float f)
{
	this->x *= f;
	this->y *= f;
	this->z *= f;
}

void vector4::operator /=(const vector4 &v)
{
	this->x /= v.x;
	this->y /= v.y;
	this->z /= v.z;
}

void vector4::operator /=(float f)
{
	this->x /= f;
	this->y /= f;
	this->z /= f;
}

vector4 vector4::operator +(const vector4 &v) const
{
	return vector4(this->x + v.x, this->y + v.y, this->z + v.z);
}

vector4 vector4::operator +(float f) const
{
	return vector4(this->x + f, this->y + f, this->z + f);
}

vector4 vector4::operator -(const vector4 &v) const
{
	return vector4(this->x - v.x, this->y - v.y, this->z - v.z);
}

vector4 vector4::operator -(float f) const
{
	return vector4(this->x - f, this->y - f, this->z - f);
}

vector4 vector4::operator *(const vector4 &v) const
{
	return vector4(this->x * v.x, this->y * v.y, this->z * v.z);
}

vector4 vector4::operator *(float f) const
{
	return vector4(this->x * f, this->y * f, this->z * f);
}

vector4 vector4::operator /(const vector4 &v) const
{
	return vector4(this->x / v.x, this->y / v.y, this->z / v.z);
}

vector4 vector4::operator /(float f) const
{
	return vector4(this->x / f, this->y / f, this->z / f);
}

vector4 vector4::operator -() const
{
	return vector4(-this->x, -this->y, -this->z);
}
