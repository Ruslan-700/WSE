#pragma once

namespace stl
{
	struct vector_bool
	{
	private:
		vector_bool **self_ptr;
		int u1;
		void *start_ptr;
		void *end_ptr;
		void *max_ptr;

		vector_bool(); // TODO: not ready for instantiation
	};
}
