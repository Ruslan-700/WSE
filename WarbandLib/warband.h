#pragma once

#include "rgl.h"
#include "wb.h"

extern wb::game_data *warband;

extern wb::cur_module_data *cur_module;
extern wb::string_manager_data *data_string_manager;
extern wb::basic_game_data *data_basic_game;

void warband_init();
