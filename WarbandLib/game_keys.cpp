#include "game_keys.h"

#include "warband.h"

using namespace wb;

bool game_key_manager::is_key_down(int game_key)
{
	return warband->input_manager.is_key_down(this->game_keys[game_key].key) || warband->input_manager.is_key_down(this->game_keys[game_key].alt_key);
}

bool game_key_manager::is_key_clicked(int game_key)
{
	return warband->input_manager.is_key_clicked(this->game_keys[game_key].key) || warband->input_manager.is_key_clicked(this->game_keys[game_key].alt_key);
}

bool game_key_manager::is_key_released(int game_key)
{
	return warband->input_manager.is_key_released(this->game_keys[game_key].key) || warband->input_manager.is_key_released(this->game_keys[game_key].alt_key);
}
