#include "WSELuaOperationsContext.h"

void prnt(std::string s)
{
	warband->log_stream.write_c_str("\n");
	warband->log_stream.write_c_str(s.c_str());
	warband->log_stream.write_c_str("\n");
}

WSELuaOperationsContext::WSELuaOperationsContext() : WSEOperationContext("array", 5100, 5199)
{
}

void WSELuaOperationsContext::OnLoad()
{
	prnt("### lua OnLoad ###");

	luaState = luaL_newstate();
	luaL_openlibs(luaState);
	//RegisterOperation("lua_call", ArrayCreate, Both, Lhs, 3, 16,
	//	"Creates an array object of <1> (0: Integer, 1: String, 2: Position) and stores its ID into <0>. You can specify up to 14 dimensions, from <2> to <15>. The array will be initialized by default with 0 / empty string / 0-position.",
	//	"destination", "type_id", "Dim 0", "Dim 1", "Dim 2", "Dim 3", "Dim 4", "Dim 5", "Dim 6", "Dim 7", "Dim 8", "Dim 9", "Dim 10", "Dim 11", "Dim 12", "Dim 13");

}

void WSELuaOperationsContext::OnUnload()
{
	lua_close(luaState);
}