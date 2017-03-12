#pragma once

#include "rgl_quaternion.h"
#include "rgl_vector4.h"
#include <string>
#include <sstream>

namespace rgl
{
	struct quaternion;

	struct orientation
	{
		vector4 s; //x
		vector4 f; //y
		vector4 u; //z
		
		bool operator ==(const orientation &r) const;
		bool operator !=(const orientation &r) const;
		quaternion to_quaternion() const;
	};

	struct matrix
	{
		orientation rot;
		vector4 o;
		
		void initialize();
		void orthonormalize();
		void rotate_x(float f);
		void rotate_y(float f);
		void rotate_z(float f);
		void scale(const vector4 &scale);
		matrix &transform_to_local(matrix &m, const matrix &parent) const;
		matrix &transform_to_parent(matrix &m, const matrix &local) const;
		vector4 &transform_dir_to_local(vector4 &v, const vector4 &parent) const;
		vector4 &transform_dir_to_parent(vector4 &v, const vector4 &local) const;
		vector4 &transform_point_to_local(vector4 &v, const vector4 &parent) const;
		vector4 &transform_point_to_parent(vector4 &v, const vector4 &local) const;
		std::string toString() const;
		void fromString(std::string str);
		bool operator ==(const matrix &m) const;
		bool operator !=(const matrix &m) const;
	};
}
