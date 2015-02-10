#include "warband.h"

wb::game_data *warband = nullptr;

wb::cur_module_data *cur_module = nullptr;
wb::string_manager_data *data_string_manager = nullptr;
wb::basic_game_data *data_basic_game = nullptr;

void warband_init()
{
	rgl::check_types();
	wb::check_types();
	warband = (wb::game_data *)(wb::addresses::game_data_ptr);

	cur_module = (wb::cur_module_data *)(wb::addresses::cur_module_data_ptr);
	data_string_manager = (wb::string_manager_data *)(wb::addresses::string_manager_data_ptr);
	data_basic_game = (wb::basic_game_data *)(wb::addresses::basic_game_data_ptr);
}
