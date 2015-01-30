#pragma once

#include "stl_vector.h"
#include "fmod.h"
#include "rgl.h"

namespace wb
{
	struct music_track
	{
		unsigned int flags;
		unsigned int continue_flags;
		unsigned int culture_flags;
		unsigned int continue_culture_flags;
		rgl::string file;
	};

	struct sound_channel
	{
		int u1[9];
	};

	struct music_manager
	{
		CRITICAL_SECTION u1;
		stl::vector<void *> u2;
		stl::vector<void *> u3;
		unsigned int situation_flags;
		int u4;
		int u5;
		int u6;
		stl::vector<void *> u7;
		sound_channel u8[100];
		int u9[100];
		FMOD_SYSTEM *fmod_system;
		rgl::matrix listener_attributes_3d;
		bool active[2];
		rgl::timer update_timer;
		int u10[2];
		int u11[2];
		FMOD_CHANNEL *fmod_channel[2];
		bool fade_out[2];
		rgl::timer fade_out_timer[2];
		int u12[2];
	};
}
