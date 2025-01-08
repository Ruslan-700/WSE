#pragma once

#include <lua.hpp>
#include "WSE.h"

enum triggerPart: int
{
	condition,
	consequence
};

void initLGameTable(lua_State *L);