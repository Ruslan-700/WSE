#pragma once

#include "stl_vector.h"

namespace stl
{
	template <typename T>
	struct priority_queue
	{
	private:
		vector<T> vec;
		int u1;

		priority_queue(); // TODO: not ready for instantiation
	};
}
