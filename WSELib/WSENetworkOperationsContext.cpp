#include "WSENetworkOperationsContext.h"

#include "WSE.h"
#include "warband.h"

void ServerSetMaxNumPlayers(WSENetworkOperationsContext *context)
{
	int max_num_players, max_num_private_players;
	
	context->ExtractValue(max_num_players);
	context->ExtractValue(max_num_private_players, max_num_players);

	if (!warband->basic_game.is_multiplayer())
		return;

	if (max_num_players < 2 || max_num_private_players < max_num_players || max_num_private_players > NUM_NETWORK_PLAYERS)
		return;

	if (max_num_private_players > WSE->SettingsIni.Int("limits", "max_num_network_players", NUM_NETWORK_PLAYERS))
		return;
	
	warband->multiplayer_data.max_num_players = max_num_players;
	warband->multiplayer_data.max_num_private_players = max_num_private_players;
}

void ServerSetPasswordAdmin(WSENetworkOperationsContext *context)
{
	rgl::string str;

	context->ExtractString(str);

	if (!warband->basic_game.is_multiplayer() || !warband->basic_game.is_safe_string(str))
		return;

	warband->multiplayer_data.server_password_admin = str;
}

void ServerSetPasswordPrivate(WSENetworkOperationsContext *context)
{
	rgl::string str;

	context->ExtractString(str);

	if (!warband->basic_game.is_multiplayer() || !warband->basic_game.is_safe_string(str))
		return;

	warband->multiplayer_data.server_password_private = str;
}

int ServerMapRotationGetNumEntries(WSENetworkOperationsContext *context)
{
#if defined WARBAND_DEDICATED
	return warband->multiplayer_data.maps_list.size();
#else
	return -1;
#endif
}

int ServerMapRotationGetIndex(WSENetworkOperationsContext *context)
{
#if defined WARBAND_DEDICATED
	return warband->multiplayer_data.cur_map_index;
#else
	return -1;
#endif
}

void ServerMapRotationSetIndex(WSENetworkOperationsContext *context)
{
#if defined WARBAND_DEDICATED
	int index;

	context->ExtractValue(index);

	if (index < 0 || index >= warband->multiplayer_data.maps_list.size())
		return;
	
	warband->multiplayer_data.cur_map_index = index;
#endif
}

int ServerMapRotationGetMap(WSENetworkOperationsContext *context)
{
#if defined WARBAND_DEDICATED
	int index;

	context->ExtractValue(index);

	if (index < 0 || index >= warband->multiplayer_data.maps_list.size())
		return -1;

	return warband->multiplayer_data.maps_list[index];
#else
	return -1;
#endif
}

void ServerMapRotationAddMap(WSENetworkOperationsContext *context)
{
#if defined WARBAND_DEDICATED
	int site_no;
	
	context->ExtractSiteNo(site_no);

	if (context->HasMoreOperands())
	{
		int index;

		context->ExtractValue(index);
		warband->multiplayer_data.maps_list.insert(index, site_no);
	}
	else
	{
		warband->multiplayer_data.maps_list.push_back(site_no);
	}
#endif
}

void ServerMapRotationRemoveMap(WSENetworkOperationsContext *context)
{
#if defined WARBAND_DEDICATED
	if (warband->multiplayer_data.maps_list.size() == 1)
		return;
	
	if (context->HasMoreOperands())
	{
		int index;

		context->ExtractValue(index);
		warband->multiplayer_data.maps_list.remove_at(index);
	}
	else
	{
		warband->multiplayer_data.maps_list.pop_back();
	}
#endif
}

int ServerGetHorseFriendlyFire(WSENetworkOperationsContext *context)
{
	return WSE->Network.GetHorseFriendlyFire();
}

void ServerSetHorseFriendlyFire(WSENetworkOperationsContext *context)
{
	bool value;

	context->ExtractBoolean(value);

	WSE->Network.SetHorseFriendlyFire(value);
}

int ServerGetShowCrosshair(WSENetworkOperationsContext *context)
{
	return WSE->Network.GetShowCrosshair();
}

void ServerSetShowCrosshair(WSENetworkOperationsContext *context)
{
	bool value;

	context->ExtractBoolean(value);

	WSE->Network.SetShowCrosshair(value);
}

int GetServerOptionAtConnect(WSENetworkOperationsContext *context)
{
	int index;

	context->ExtractValue(index);

	if (!warband->basic_game.is_client())
		return -1;

	const char *connected_server_ip = inet_ntoa(warband->multiplayer_data.server_address.ip);
	int connected_server_port = warband->multiplayer_data.server_address.port;

	for (int i = 0; i < warband->multiplayer_data.server_list.size(); ++i)
	{
		wb::multiplayer_server *server = &warband->multiplayer_data.server_list[i];

		if (server->ip == connected_server_ip && server->port == connected_server_port)
		{
			if (index >= 0 && index < server->module_settings.size())
				return server->module_settings[index];
			else
				return -1;
		}
	}

	return -1;
}

WSENetworkOperationsContext::WSENetworkOperationsContext() : WSEOperationContext("network", 3500, 3599)
{
}

void WSENetworkOperationsContext::OnLoad()
{
	ReplaceOperation(477, "server_set_anti_cheat", nullptr, Both, Disable|Undocumented, 0, 0, "Disabled by WSE");
	
	ReplaceOperation(491, "server_set_max_num_players", ServerSetMaxNumPlayers, Both, None, 1, 2,
		"Sets maximum players to <0> and maximum private players to <1> (default = same as <0>). Both values must be in the range 2-250, <1> can't be lower than <0>",
		"max_players", "max_private_players");

	RegisterOperation("server_set_password_admin", ServerSetPasswordAdmin, Both, None, 1, 1,
		"Sets <0> as server administrator password",
		"password");

	RegisterOperation("server_set_password_private", ServerSetPasswordPrivate, Both, None, 1, 1,
		"Sets <0> as server private player password",
		"password");

	RegisterOperation("server_map_rotation_get_count", ServerMapRotationGetNumEntries, Server, Lhs, 1, 1,
		"Stores the number of maps in rotation into <0>",
		"destination");

	RegisterOperation("server_map_rotation_get_index", ServerMapRotationGetIndex, Server, Lhs, 1, 1,
		"Stores the current map rotation index into <0>",
		"destination");

	RegisterOperation("server_map_rotation_set_index", ServerMapRotationSetIndex, Server, None, 1, 1,
		"Sets the current rotation index to <0>",
		"index");

	RegisterOperation("server_map_rotation_get_map", ServerMapRotationGetMap, Server, Lhs, 2, 2,
		"Stores the map at <1> into <0>",
		"destination", "index");

	RegisterOperation("server_map_rotation_add_map", ServerMapRotationAddMap, Server, None, 1, 2,
		"Adds <0> to the map rotation at <1>",
		"site_no", "index");

	RegisterOperation("server_map_rotation_remove_map", ServerMapRotationRemoveMap, Server, None, 0, 1,
		"Removes the map at <0> from the map rotation (does not work when only one left)",
		"index");

	RegisterOperation("server_get_horse_friendly_fire", ServerGetHorseFriendlyFire, Both, Lhs|BreakNetwork, 1, 1,
		"Stores horse friendly fire status into <0>",
		"destination");

	RegisterOperation("server_set_horse_friendly_fire", ServerSetHorseFriendlyFire, Both, BreakNetwork, 1, 1,
		"Enables or disables horse friendly fire",
		"value");

	RegisterOperation("server_get_show_crosshair", ServerGetShowCrosshair, Both, Lhs|BreakNetwork, 1, 1,
		"Stores crosshair visibility status into <0>",
		"destination");

	RegisterOperation("server_set_show_crosshair", ServerSetShowCrosshair, Both, BreakNetwork, 1, 1,
		"Enables or disables the crosshair",
		"value");

	RegisterOperation("get_server_option_at_connect", GetServerOptionAtConnect, Both, Lhs, 1, 2,
		"Stores option <1> into <0>",
		"destination", "index");
}
