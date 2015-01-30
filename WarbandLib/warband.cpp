#include "warband.h"

wb::game_data *warband = nullptr;

void warband_init()
{
	rgl::check_types();
	wb::check_types();
	warband = (wb::game_data *)(wb::addresses::game_data_ptr);
}
