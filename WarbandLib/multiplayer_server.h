#pragma once

#include "rgl.h"

namespace wb
{
	struct multiplayer_server
	{
		rgl::string ip;
		int ping;
		int port;
		int site_no;
		int mission_template_no;
		unsigned __int64 vac_id;
		rgl::string name;
		rgl::string module_name;
		rgl::string map_name;
		rgl::string game_type_name;
		int players;
		int max_players;
		int passworded;
		int dedicated;
		int compatible_game_version;
		int compatible_module_version;
		stl::vector<int> module_settings;
		float last_try_time;
		int num_tries;

		multiplayer_server();
	};
}
