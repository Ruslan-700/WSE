#include "rgl_quaternion.h"

using namespace rgl;

orientation quaternion::to_orientation() const
{
	rgl::orientation rot;
	
	rot.s.x = 1.0f - (z * (2 * z) + y * (2 * y));
	rot.s.y = (w * (2 * z) + x * (2 * y));
	rot.s.z = (x * (2 * z) - w * (2 * y));
	rot.f.x = (x * (2 * y) - w * (2 * z));
	rot.f.y = 1.0f - (z * (2 * z) + x * (2 * x));
	rot.f.z = (w * (2 * x) + y * (2 * z));
	rot.u.x = (x * (2 * z) + w * (2 * y));
	rot.u.y = (y * (2 * z) - w * (2 * x));
	rot.u.z = 1.0f - (x * (2 * x) + y * (2 * y));
	return rot;
}
