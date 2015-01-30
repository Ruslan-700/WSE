#pragma once

#include "rgl.h"
#include "rgl_body_part.h"

namespace rgl
{
	typedef void (__cdecl *CheckIntersection_func)(body_part *part1, body_part *part2, int max_num_checks, intersection_point *points, int &num_points);
	
	struct functions
	{
		static CheckIntersection_func check_intersection;
	};
}
