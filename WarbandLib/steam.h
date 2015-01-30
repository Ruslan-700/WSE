#pragma once

#include "rgl.h"

#define NUM_STEAM_ACHIEVEMENTS 81

namespace wb
{
	struct steam_callback
	{
		int u1;
		bool u2;
		int u3;
		int u4;
		int u5;
	};

	struct steam_manager
	{
		steam_callback u1[6];
	};

	struct steam_achievement
	{
		int id;
		rgl::string name;
		bool awarded;
		bool u1;
		int num_stats;
		stl::vector<int> stats;
	};

	struct steam_achievement_manager
	{
		steam_callback u1[3];
		steam_achievement achievements[NUM_STEAM_ACHIEVEMENTS];
		int u2;
		int u3;
		void *steam_user;
		void *steam_user_stats;
		bool stats_received;
	};
}
