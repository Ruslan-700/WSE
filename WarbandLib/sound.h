#pragma once

#include "fmod.h"
#include "rgl.h"

#define NUM_MAPPED_SOUNDS 130

namespace wb
{
	enum sound_flags
	{
		sf_2d                  = 0x1,
		sf_looping             = 0x2,
		sf_start_at_random_pos = 0x4,
		sf_priority_mask       = 0xF0,
		sf_volume_mask         = 0xF00,
	};

	struct sound
	{
		rgl::string id;
		unsigned int flags;
		int sample_nos[32];
		unsigned int sample_faction_nos[32];
		int num_samples;
	};

	struct sound_sample
	{
		FMOD_SOUND *fmod_sound;
		unsigned int flags;
		float length;
		float frequency;
		int pcm_samples;
	};

	struct sound_manager
	{
		int num_sounds;
		sound *sounds;
		int mapped_sounds[NUM_MAPPED_SOUNDS];
		float sound_distances[NUM_MAPPED_SOUNDS];
	};
}
