#pragma once

#include "rgl.h"
#include "simple_trigger.h"

#define NUM_MAPPED_PRESENTATIONS 7

namespace wb
{
	struct presentation
	{
		rgl::string id;
		unsigned int flags;
		simple_trigger_manager simple_triggers;
		int mesh_no;
	};

	struct presentation_manager
	{
		presentation *presentations;
		int num_presentations;
		int mapped_presentations[NUM_MAPPED_PRESENTATIONS];

		int addPresentation(const presentation &newPresentation);
		bool removePresentation(int index);
	};
}
