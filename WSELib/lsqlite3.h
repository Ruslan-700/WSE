#pragma once

#include "lua.h"

typedef char* (*str_callback)(const char* path);

LUALIB_API int luaopen_lsqlite3(lua_State *L);
void lsqlite3_set_sandboxed_path(str_callback callback);