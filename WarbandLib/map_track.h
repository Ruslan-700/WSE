#pragma once

#include "rgl.h"

namespace wb
{
	enum map_track_flags
	{
		mtrf_troop_count_mask    = 0xFF,
		mtrf_party_template_mask = 0xFF00,
		mtrf_range_mask          = 0xF0000,
		mtrf_battle_track        = 0x10000000,
		mtrf_spotted             = 0x20000000,
	};
	
	struct map_track
	{
		rgl::vector4 position;
		float rotation;
		float age;
		unsigned int flags;
		rgl::matrix final_position;
	};
}
