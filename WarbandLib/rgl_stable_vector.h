#pragma once

#include "stl_vector.h"

#define rgl_sv_chunk_size 16
#define rgl_sv_chunk_bits 4
#define rgl_sv_chunk_mask 0xF

namespace rgl
{
	template <typename T>
	struct stable_vector
	{
		int num_items;
		int num_ids;
		stl::vector<int> ids;
		stl::vector<T *> items;
		int hint_number;
		int num_created;

		int size() const
		{
			return num_items;
		}

		bool is_valid_index(int index)
		{
			if (index < 0 || index > this->num_ids)
				return false;

			return operator[](index).valid != 0;
		}

		int get_first_valid_index()
		{
			int index = 0;

			for (; index < this->num_items; ++index)
			{
				if ((*this)[index].valid)
					break;
			}

			return index;
		}

		int get_next_valid_index(int index)
		{
			for (++index; index < this->num_items; ++index)
			{
				if ((*this)[index].valid)
					break;
			}

			return index;
		}

		T &operator[](int index)
		{
			return this->items[index >> rgl_sv_chunk_bits][index & rgl_sv_chunk_mask];
		}
	};
}
