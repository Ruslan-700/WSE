#pragma once

#include "rgl.h"
#include "basics.h"

namespace wb
{
	struct quest
	{
		rgl::string title;
		rgl::string description;
		rgl::string giver_name;
		rgl::string cur_description;
		rgl::string cur_title;
		unsigned int flags;
		rgl::string id;
		unsigned int progression;
		note_manager notes;
		int troop;
		int item;
		int number;
		int giver_troop_no;
		float start_date;
		int u1;
		int u2;
		slot_manager slots;
	};
}
