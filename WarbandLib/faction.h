#pragma once

#include "rgl.h"
#include "basics.h"

namespace wb
{
	struct faction
	{
		rgl::string name;
		bool renamed;
		rgl::string id;
		unsigned int flags;
		unsigned int colors;
		float relations[128];
		int num_ranks;
		int u1;
		stl::vector<rgl::string> ranks;
		float u2;
		float max_relation_with_player;
		slot_manager slots;
		note_manager notes;
	};
}
