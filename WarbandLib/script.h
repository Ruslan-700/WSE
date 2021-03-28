#pragma once

#include "rgl.h"
#include "operation.h"

#define SCRIPT_GAME_QUICK_START 1
#define SCRIPT_GAME_GET_SCENE_NAME 37
#define SCRIPT_GAME_GET_MISSION_TEMPLATE_NAME 38
#define SCRIPT_GAME_GET_MULTIPLAYER_SERVER_OPTION_FOR_MISSION_TEMPLATE 40
#define SCRIPT_GAME_MULTIPLAYER_SERVER_OPTION_FOR_MISSION_TEMPLATE_TO_STRING 41
#define SCRIPT_GAME_MISSILE_DIVES_INTO_WATER 52
#define NUM_MAPPED_SCRIPTS 55

namespace wb
{
	struct script
	{
		int u1;
		rgl::string id;
		float interval;
		operation_manager operations;

		bool execute(const int &num_params, __int64 *params);
		static void error(const char *message);
	};

	struct script_manager
	{
		script *scripts;
		int num_scripts;
		int num_simple_trigger_scripts;
		int mapped_scripts[NUM_MAPPED_SCRIPTS];
	};
}
