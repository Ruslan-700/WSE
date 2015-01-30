#pragma once

#include "rgl_vector2.h"

namespace rgl
{
	struct matrix;

	struct vector3
	{
		float x;
		float y;
		float z;
	};

	struct vector4
	{
		static const vector4 zero;
		static const vector4 one;

		float x;
		float y;
		float z;
		float w;
		
		vector4(float f = 0.0f, float w = 0.0f);
		vector4(float x, float y, float z);
		vector4(float x, float y, float z, float w);
		void rotate_x(float f);
		void rotate_y(float f);
		void rotate_z(float f);
		float length() const;
		float length_squared() const;
		void normalize();
		void cross(const vector4 &v1, const vector4 &v2);
		float dot(const vector4 &v) const;
		bool is_unit() const;
		void transform(const vector4 &v, const matrix &m);
		vector2 vec2() const;
		bool operator ==(const vector4 &v) const;
		bool operator !=(const vector4 &v) const;
		void operator +=(const vector4 &v);
		void operator +=(float f);
		void operator -=(const vector4 &v);
		void operator -=(float f);
		void operator *=(const vector4 &v);
		void operator *=(float f);
		void operator /=(const vector4 &v);
		void operator /=(float f);
		vector4 operator +(const vector4 &v) const;
		vector4 operator +(float f) const;
		vector4 operator -(const vector4 &v) const;
		vector4 operator -(float f) const;
		vector4 operator *(const vector4 &v) const;
		vector4 operator *(float f) const;
		vector4 operator /(const vector4 &v) const;
		vector4 operator /(float f) const;
		vector4 operator -() const;
	};
}
