#include "WSELuaOperationsContext.h"

WSELuaOperationsContext::WSELuaOperationsContext() : WSEOperationContext("array", 5100, 5199)
{
	lua_State *state = luaL_newstate();
	luaL_openlibs(state);
	lua_close(state);
}

void WSELuaOperationsContext::OnLoad()
{
}

void WSELuaOperationsContext::OnUnload()
{
}