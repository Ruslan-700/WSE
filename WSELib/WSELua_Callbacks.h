#pragma once

#include <lua.hpp>
#include "WSE.h"

void initLGameTable(lua_State *L);
void register_wse_require_loader(lua_State *L);

//Collect callback into vector for registering later
//The structs are an ugly hack, but it's the only way I found
//Basically, this is to execute the code at file level
#define REG(NAME) \
	int lc_##NAME(lua_State* L); \
	static struct Register_##NAME{ \
		Register_##NAME() { _G_game_callbacks.emplace_back(#NAME, lc_##NAME); } \
	} register_##NAME;

//Allow double naming
#define REG2(NAME, ALIAS) \
	int lc_##NAME(lua_State* L); \
	static struct Register_##NAME{ \
		Register_##NAME() { _G_game_callbacks.emplace_back(#NAME, lc_##NAME); _G_game_callbacks.emplace_back(#ALIAS, lc_##NAME); } \
	} register_##NAME;

//For global callbacks, that are not in game
#define GREG(NAME) \
	int lc_##NAME(lua_State* L); \
	static struct Register_##NAME{ \
		Register_##NAME() { _G_callbacks.emplace_back(#NAME, lc_##NAME); } \
	} register_##NAME;

#define GREG2(NAME, ALIAS) \
	int lc_##NAME(lua_State* L); \
	static struct Register_##NAME{ \
	Register_##NAME() { _G_callbacks.emplace_back(#NAME, lc_##NAME); _G_callbacks.emplace_back(#ALIAS, lc_##NAME); } \
} register_##NAME;