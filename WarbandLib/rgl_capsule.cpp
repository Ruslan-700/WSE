#include "rgl_capsule.h"
#include "rgl.h"

using namespace rgl;

capsule::capsule(float radius, const vector4 &center, const vector4 &direction)
{
	THISCALL3(rgl::addresses::capsule_Ctor, this, radius, center, direction);
}

capsule::~capsule()
{
}

int capsule::get_type() const
{
	UNREACHABLE;
	return 0;
}

const char *capsule::get_name() const
{
	UNREACHABLE;
	return NULL;
}

void capsule::read(const io_stream &stream)
{
	UNREACHABLE;
}

void capsule::write(const io_stream &stream)
{
	UNREACHABLE;
}

body_part *capsule::copy()
{
	UNREACHABLE;
	return NULL;
}

void capsule::unknown_1(int)
{
	UNREACHABLE;
}

void capsule::scale(const vector4 &scale)
{
	UNREACHABLE;
}

void capsule::transform(const matrix &matrix)
{
	UNREACHABLE;
}

void capsule::set_entity_no(const int &id)
{
	UNREACHABLE;
}

const vector4 &capsule::get_b_min()
{
	UNREACHABLE;
	return vector4::zero;
}

const vector4 &capsule::get_b_max()
{
	UNREACHABLE;
	return vector4::zero;
}

vector4 &capsule::store_min(vector4 &v)
{
	UNREACHABLE;
	return b_min;
}

vector4 &capsule::store_max(vector4 &v)
{
	UNREACHABLE;
	return b_max;
}

void capsule::update_min_max()
{
	UNREACHABLE;
}

void capsule::set_flags(const unsigned int &flag, const bool &update_physics_body)
{
	UNREACHABLE;
}

void capsule::set_flag(const unsigned int &flag, const bool &update_physics_body)
{
	UNREACHABLE;
}

void capsule::clear_flag(const unsigned int &flag, const bool &update_physics_body)
{
	UNREACHABLE;
}
