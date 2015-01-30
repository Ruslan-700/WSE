#pragma once

#include "rgl_matrix.h"

namespace rgl
{
	struct orientation;

	struct quaternion
	{
		float w;
		float x;
		float y;
		float z;

		orientation to_orientation() const;
	};
}
