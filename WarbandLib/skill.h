#pragma once

#include "rgl.h"
#include "operation.h"

#define NUM_SKILLS 42

namespace wb
{
	enum mbSkillFlags
	{
		sf_effects_party = 0x10,
		sf_inactive = 0x100,
	};

	struct skill
	{
		rgl::string description;
		rgl::string name;
		rgl::string id;
		int u1;
		int max_level;
		int u2;
		int u3;
		unsigned int flags;
	};
}
