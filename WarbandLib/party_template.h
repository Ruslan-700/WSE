#pragma once

#include "rgl.h"
#include "basics.h"

namespace wb
{
	struct party_template_stack
	{
		int troop_no;
		int num_min_troops;
		int num_max_troops;
		unsigned int flags;
	};

	struct party_template
	{
		rgl::string name;
		rgl::string id;
		unsigned __int64 flags;
		int menu_no;
		int faction_no;
		int personality;
		int num_parties_created;
		int num_parties_destroyed;
		int num_parties_destroyed_by_player;
		party_template_stack stacks[6];
		slot_manager slots;
	};
}
