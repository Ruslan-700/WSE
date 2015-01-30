#include "missile.h"

using namespace wb;

rgl::vector4 missile::get_velocity()
{
	return (path[cur_path] - path[prev_path]) * 50.0f;
}

rgl::vector4 missile::get_direction()
{
	rgl::vector4 dir = get_velocity();

	dir.normalize();
	return dir;
}
