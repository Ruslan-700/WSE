#pragma once

#include "rgl.h"

namespace wb
{
	struct simple_game_object
	{
		int valid;
		int no;
		int sequence;

		virtual ~simple_game_object();
		virtual void initialize();
	};
}
