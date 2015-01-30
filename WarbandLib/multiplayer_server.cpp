#include "multiplayer_server.h"

#include "warband.h"

using namespace wb;

multiplayer_server::multiplayer_server()
{
	this->last_try_time = 0.0;
	this->port = -1;
	this->site_no = -1;
	this->mission_template_no = -1;
	this->ping = 0;
	this->vac_id = 0;
	this->players = 0;
	this->max_players = 0;
	this->passworded = 0;
	this->dedicated = 0;
	this->compatible_game_version = 0;
	this->compatible_module_version = 0;
	this->num_tries = 0;
};
