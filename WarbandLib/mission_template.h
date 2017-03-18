#pragma once

#include "rgl.h"
#include "trigger.h"

namespace wb
{
	struct mission_template_group
	{
		int entry_point_no;
		unsigned int flags;
		unsigned int cur_alter_flags;
		int u1[2];
		int num_troops;
		int u2[2];
		unsigned int ai_flags;
		int num_cur_item_overrides;
		int cur_item_overrides[10];
		unsigned int alter_flags;
		int num_item_overrides;
		int item_overrides[10];
		int u3;
	};

	struct mission_template
	{
		rgl::string id;
		rgl::string name;
		unsigned int flags;
		rgl::string description;
		mission_template_group *groups;
		int num_groups;
		int u1[4];
		trigger_manager triggers;

		int addTrigger(const trigger &newTrigger, int templateNo);
		bool removeTrigger(int index);
	};
}
