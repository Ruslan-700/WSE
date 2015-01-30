#pragma once

#include "fmod.h"
#include "rgl.h"
#include "simple_game_object.h"

namespace wb
{
	struct map_event : simple_game_object
	{
		int type;
		rgl::string extra_text;
		rgl::vector2 position;
		rgl::vector2 land_position;
		float u1[2];
		float height;
		float radius;
		float age;
		rgl::quad *quad;
		rgl::quad *land_quad;
		rgl::strategic_entity *entity;
		int attacker_party_no;
		int defender_party_no;
		rgl::timer simulation_timer;
		float simulation_time;
		int u2;
		int num_troops;
		rgl::timer sound_timer;
		FMOD_CHANNEL *fmod_channel;
	};
}
