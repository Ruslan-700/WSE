#pragma once

#include "rgl.h"

#define NUM_GAME_KEYS 52

namespace wb
{
	struct game_key
	{
		unsigned int key;
		unsigned int alt_key;
	};

	struct game_key_manager
	{
		game_key game_keys[NUM_GAME_KEYS];
		game_key stored_game_keys[NUM_GAME_KEYS];

		bool is_key_down(int game_key);
		bool is_key_clicked(int game_key);
		bool is_key_released(int game_key);
	};
}
