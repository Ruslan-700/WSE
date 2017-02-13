#pragma once

#include <lua.hpp>

#include "WSEOperationContext.h"

class WSELuaOperationsContext : public WSEOperationContext
{
public:
	WSELuaOperationsContext();

protected:
	lua_State *luaState;

	virtual void OnLoad();
	virtual void OnUnload();
};