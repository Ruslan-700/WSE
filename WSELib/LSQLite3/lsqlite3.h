#pragma once

#include "lua.h"

#ifdef __cplusplus
extern "C"{
#endif 
	LUALIB_API int luaopen_lsqlite3(lua_State *L);
	void lsqlite3_set_sandboxed_path_callback(str_callback callback);
#ifdef __cplusplus
}
#endif