#pragma once

#include "rgl.h"
#include "face_generator.h"
#include "multiplayer_server.h"

namespace wb
{
	enum server_list_type
	{
		slt_internet = 0,
		slt_lan = 1,
		slt_favorites = 2,
	};

	struct multiplayer_profile
	{
		rgl::string name;
		int gender_no;
		int banner_no;
		face_keys face_keys;
		stl::vector<multiplayer_server> favorites;
		stl::vector<multiplayer_server> server_list;
		server_list_type server_list_type;
		int filter_servers;
		int filter_num_players;
		int u1;
		int filter_password;
		int u2;
		int filter_ping;
		int filter_game_type;
		int filter_version;
	};

	struct multiplayer_profile_manager
	{
		int cur_profile_no;
		stl::vector<multiplayer_profile> profiles;
	};
}
