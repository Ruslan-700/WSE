#pragma once

#include "rgl.h"
#include "simple_game_object.h"

namespace wb
{
	struct fading_out_entity : simple_game_object
	{
		rgl::strategic_entity *entity;
		rgl::timer fade_timer;
		float fade_duration;
		bool remove_entity;
		bool fade_in;
	};
}
