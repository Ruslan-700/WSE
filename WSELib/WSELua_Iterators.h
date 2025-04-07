#include <lua.hpp>
#include "WSE.h"

struct gameIterator
{
	bool valid;

	void(*advance)(gameIterator *it);
	bool(*curValIsValid)(gameIterator *it);

	int curVal;

	rgl::matrix pos;
	float radius;
	bool positional_succ;
	wb::mission_grid_iterator grid_iterator;

	int subKindNo;
	int metaType;
};

int lIterNext(lua_State *L)
{
	gameIterator *it = (gameIterator*)lua_touserdata(L, lua_upvalueindex(1));

	if (it && it->curValIsValid(it))
	{
		lua_pushnumber(L, it->curVal);

		it->advance(it);
	}
	else
	{
		if (it)
			it->valid = false;

		lua_pushnil(L);
	}

	return 1;
}

int lPushIterator(lua_State *L, const gameIterator &it)
{
	gameIterator *ud = (gameIterator*)lua_newuserdata(L, sizeof(gameIterator));
	*ud = it;

	lua_pushcclosure(L, lIterNext, 1);
	return 1;
}

void lPartiesIterAdvance(gameIterator *it)
{
	it->curVal = warband->cur_game->parties.get_next_valid_index(it->curVal);
}

bool lPartiesIterCurValIsValid(gameIterator *it)
{
	return it->curVal < warband->cur_game->parties.num_created;
}

void lAgentsIterAdvance_pos(gameIterator *it)
{
	it->curVal = warband->cur_mission->agents.get_next_valid_index(it->curVal);

	for (; it->curVal < warband->cur_mission->agents.size(); it->curVal = warband->cur_mission->agents.get_next_valid_index(it->curVal))
	{
		wb::agent *agent = &warband->cur_mission->agents[it->curVal];
		if ((it->pos.o - agent->position).length() <= it->radius){
			return;
		}
	}
	it->positional_succ = false;
}

void lAgentsIterAdvance_grid(gameIterator *it)
{
	if (warband->cur_mission->grid.advance_iterator(it->grid_iterator))
	{
		it->curVal = it->grid_iterator.agent_obj->agent->no;
	}
	else
	{
		it->positional_succ = false;
	}
}

void lAgentsIterAdvance(gameIterator *it)
{
	it->curVal = warband->cur_mission->agents.get_next_valid_index(it->curVal);
}

bool lAgentsIterCurValIsValid_positional(gameIterator *it)
{
	return it->positional_succ;
}

bool lAgentsIterCurValIsValid(gameIterator *it)
{
	return it->curVal < warband->cur_mission->agents.size();
}

void lPropInstIterAdvance(gameIterator *it)
{
	it->curVal = warband->cur_mission->mission_objects.get_next_valid_index(it->curVal);
	for (; it->curVal < warband->cur_mission->mission_objects.size(); it->curVal = warband->cur_mission->mission_objects.get_next_valid_index(it->curVal))
	{
		wb::mission_object *mission_object = &warband->cur_mission->mission_objects[it->curVal];

		if ((it->subKindNo <= 0 || mission_object->sub_kind_no == it->subKindNo) && (it->metaType <= 0 || mission_object->meta_type == it->metaType - 1))
			break;
	}
}

bool lPropInstIterCurValIsValid(gameIterator *it)
{
	return it->curVal < warband->cur_mission->mission_objects.size();
}

void lPlayersIterAdvance(gameIterator *it)
{
	for (it->curVal++; it->curVal < NUM_NETWORK_PLAYERS; it->curVal++)
	{
		wb::network_player *player = &warband->multiplayer_data.players[it->curVal];

		if (player->is_active())
			break;
	}
}

bool lPlayersIterCurValIsValid(gameIterator *it)
{
	return it->curVal < NUM_NETWORK_PLAYERS;
}

struct file_iterator
{
	bool valid;
	WIN32_FIND_DATA FindFileData;
	HANDLE hFind;
};

int lFileIterator_Free(lua_State *L)
{
	file_iterator* data = (file_iterator*)lua_touserdata(L, 1);
	FindClose(data->hFind);

	return 0;
}

int lFileIterator_Next(lua_State *L)
{
	file_iterator *it = (file_iterator*)lua_touserdata(L, lua_upvalueindex(1));

	if (it && it->valid)
	{
		lua_pushstring(L, it->FindFileData.cFileName);
		lua_pushinteger(L, it->FindFileData.dwFileAttributes);

		if ( !FindNextFile(it->hFind, &(it->FindFileData)) ) it->valid = false;

		return 2;
	}
	else
	{
		lua_pushnil(L);
		return 1;
	}
}

int lFileIterator_Push(lua_State* L, const char* path)
{
	file_iterator* it = (file_iterator*)lua_newuserdata(L, sizeof(file_iterator));
	
	//We will create a HANDLE and need to free it when our iterator gets garbace collected
	if (luaL_newmetatable(L, "file_iterator_mt")) //returns 1 if not yet created. Leaves mt on stack in any case. The metatable will be stored in lua registry
	{
		lua_pushcfunction(L, lFileIterator_Free);
		lua_setfield(L, -2, "__gc");
	}
	lua_setmetatable(L, -2);

	it->hFind = FindFirstFile(path, &(it->FindFileData));
	if (it->hFind == INVALID_HANDLE_VALUE)
	{
		it->valid = false;
		return 0;
	}

	it->valid = true;
	lua_pushcclosure(L, lFileIterator_Next, 1);
	return 1;
}