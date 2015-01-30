#pragma once

#include "rgl.h"

#define NUM_SLOTS 0x100000

namespace wb
{
	struct slot_manager
	{
		stl::vector<__int64> slots;

		__int64 get(int index)
		{
			if (index >= slots.size())
				return 0;

			return this->slots[index];
		}

		void set(int index, const __int64 &value)
		{
			resize(index + 1);
			slots[index] = value;
		}

		void resize(int new_size)
		{
			int cur_size = slots.size();

			if (new_size <= cur_size)
				return;
			
			for (int i = 0x10; i <= 0x10000; i <<= 1)
			{
				if (new_size < i)
				{
					new_size = i;
					break;
				}
			}

			slots.resize(new_size);

			for (int i = cur_size; i < new_size; ++i)
			{
				slots[i] = 0;
			}
		}
	};

	struct note_manager
	{
		int values[16];
		rgl::string texts[16];
		int tableau_material_no;
		bool available;
	};
}