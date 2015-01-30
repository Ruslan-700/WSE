#pragma once

#include "stl_vector.h"

namespace rgl
{
	template <typename T>
	struct recycled_vector
	{
		stl::vector<T> free_data;
		stl::vector<T> data;
	};
}
