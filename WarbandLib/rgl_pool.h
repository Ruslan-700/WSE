#pragma once

#include "stl_list.h"

namespace rgl
{
	template <typename T>
	struct pool
	{
		stl::list<T *> items;
		int batch_size;
		T *free_items;
	};
}
