#pragma once

#define rgl_static_deque_max_size 1024

namespace rgl
{
	template <typename T>
	struct static_deque
	{
		T items[rgl_static_deque_max_size];
		int head;
		int size;
	};
}
