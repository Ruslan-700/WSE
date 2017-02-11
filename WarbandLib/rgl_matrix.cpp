#include "rgl_matrix.h"

#include <cmath>

using namespace rgl;

bool orientation::operator ==(const orientation &r) const
{
	return this->s == r.s && this->f == r.f && this->u == r.u;
}

bool orientation::operator !=(const orientation &r) const
{
	return !(*this == r);
}

quaternion orientation::to_quaternion() const
{
	quaternion q;
	float trace = s.x + f.y + u.z;
	float t;
	float t2;

	if (trace > 0.0f)
	{
		t = sqrt(trace + 1.0f);
		t2 = 0.5f / t;

		q.w = t * 0.5f;
		q.x = (f.z - u.y) * t2;
		q.y = (u.x - s.z) * t2;
		q.z = (s.y - f.x) * t2;
	}
	else
	{
		if (s.x >= f.y && s.x >= u.z)
		{
			t = sqrt(s.x - u.z - f.y + 1.0f);
			t2 = t == 0.0f ? t : 0.5f / t;

			q.w = (f.z - u.y) * t2;
			q.x = t * 0.5f;
			q.y = (s.y + f.x) * t2;
			q.z = (s.z + u.x) * t2;
		}
		else if (f.y >= u.z)
		{
			t = sqrt(f.y - s.x - u.z + 1.0f);
			t2 = t == 0.0f ? t : 0.5f / t;

			q.w = (u.x - s.z) * t2;
			q.x = (f.x + s.y) * t2;
			q.y = t * 0.5f;
			q.z = (f.z + u.y) * t2;
		}
		else
		{
			t = sqrt(u.z - f.y - s.x + 1.0f);
			t2 = t == 0.0f ? t : 0.5f / t;

			q.w = (s.y - f.x) * t2;
			q.x = (u.x + s.z) * t2;
			q.y = (u.y + f.z) * t2;
			q.z = t * 0.5f;
		}
	}

	return q;
}

void matrix::initialize()
{
	this->rot.s = vector4(1.0f, 0.0f, 0.0f);
	this->rot.f = vector4(0.0f, 1.0f, 0.0f);
	this->rot.u = vector4(0.0f, 0.0f, 1.0f);
	this->o = vector4(0.0f, 0.0f, 0.0f);
}

void matrix::orthonormalize()
{
	this->rot.f.normalize();
	this->rot.s.cross(this->rot.f, this->rot.u);
	this->rot.s.normalize();
	this->rot.u.cross(this->rot.s, this->rot.f);
}

void matrix::rotate_x(float f)
{
	float cos = std::cos(f);
	float sin = std::sin(f);
	vector4 b_old = this->rot.f;
	vector4 c_old = this->rot.u;

	this->rot.f = c_old * sin + b_old * cos;
	this->rot.u = c_old * cos - b_old * sin;
}

void matrix::rotate_y(float f)
{
	float cos = std::cos(f);
	float sin = std::sin(f);
	vector4 a_old = this->rot.s;
	vector4 c_old = this->rot.u;
	
	this->rot.s = a_old * cos - c_old * sin;
	this->rot.u = a_old * sin + c_old * cos;
}

void matrix::rotate_z(float f)
{
	float cos = std::cos(f);
	float sin = std::sin(f);
	vector4 a_old = this->rot.s;
	vector4 b_old = this->rot.f;

	this->rot.s = b_old * sin + a_old * cos;
	this->rot.f = b_old * cos - a_old * sin;
}

void matrix::scale(const vector4 &scale)
{
	this->rot.s *= scale.x;
	this->rot.f *= scale.y;
	this->rot.u *= scale.z;
}

matrix &matrix::transform_to_local(matrix &m, const matrix &parent) const
{
	matrix temp = m;
	this->transform_point_to_local(temp.o, parent.o);
	this->transform_dir_to_local(temp.rot.s, parent.rot.s);
	this->transform_dir_to_local(temp.rot.f, parent.rot.f);
	this->transform_dir_to_local(temp.rot.u, parent.rot.u);
	m = temp;

	return m;
}

matrix &matrix::transform_to_parent(matrix &m, const matrix &local) const
{
	this->transform_point_to_parent(m.o, local.o);
	this->transform_dir_to_parent(m.rot.s, local.rot.s);
	this->transform_dir_to_parent(m.rot.f, local.rot.f);
	this->transform_dir_to_parent(m.rot.u, local.rot.u);
	return m;
}

vector4 &matrix::transform_dir_to_local(vector4 &v, const vector4 &parent) const
{
	v.x = this->rot.s.dot(parent);
	v.y = this->rot.f.dot(parent);
	v.z = this->rot.u.dot(parent);
	return v;
}

vector4 &matrix::transform_dir_to_parent(vector4 &v, const vector4 &local) const
{
	v = this->rot.s * local.x + this->rot.f * local.y + this->rot.u * local.z;
	return v;
}

vector4 &matrix::transform_point_to_local(vector4 &v, const vector4 &parent) const
{
	return transform_dir_to_local(v, parent - this->o);
}

vector4 &matrix::transform_point_to_parent(vector4 &v, const vector4 &local) const
{
	vector4 temp = this->o;
	transform_dir_to_parent(v, local);
	v += temp;
	return v;
}

std::string matrix::toString() const
{
	float data[9] = { o.x, o.y, o.z,
		rot.f.x, rot.f.y, rot.f.z,
		rot.u.x, rot.u.y, rot.u.z };

	std::string s = "";
	for (int i = 0; i < 9; i++){
		int val = reinterpret_cast<int &>(data[i]);
		s += std::to_string(val);

		if (i < 8)
			s += ',';
	}

	return s;
}

void matrix::fromString(std::string str)
{
	std::stringstream ss(str);
	std::string item;

	float data[9];
	int i = 0;

	while (std::getline(ss, item, ',')) {
		int valInt = std::stoi(item);
		data[i] = reinterpret_cast<float &>(valInt);
		i++;
	}

	o = rgl::vector4(data[0], data[1], data[2]);
	rot.f = rgl::vector4(data[3], data[4], data[5]);
	rot.u = rgl::vector4(data[6], data[7], data[8]);

	orthonormalize();
}

bool matrix::operator ==(const matrix &m) const
{
	return this->rot == m.rot && this->o == m.o;
}

bool matrix::operator !=(const matrix &m) const
{
	return !(*this == m);
}
