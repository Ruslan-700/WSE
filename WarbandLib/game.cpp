#include "game.h"

#include "warband.h"

using namespace wb;

bool game::execute_script(int game_script_no, int param_1, int param_2, int param_3, int param_4, int param_5, int param_6) const
{
	int script_no = warband->script_manager.mapped_scripts[game_script_no];

	if (script_no < 0)
		return false;

	__int64 params[16] = {
		param_1,
		param_2,
		param_3,
		param_4,
		param_5,
		param_6,
	};
	return warband->script_manager.scripts[script_no].execute(6, params);
};
