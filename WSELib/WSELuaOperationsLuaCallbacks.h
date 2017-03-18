#pragma once

#include <lua.hpp>
#include "WSE.h"

int lGameExecOperationHandler(lua_State *L);
int lGetRegHandler(lua_State *L);
int lSetRegHandler(lua_State *L);
int lGetScriptNo(lua_State *L);
int lGetCurTemplateId(lua_State *L);
int lAddTrigger(lua_State *L);
int lRemoveTrigger(lua_State *L);

int lPartiesIterInit(lua_State *L);
int lAgentsIterInit(lua_State *L);
int lPropInstIterInit(lua_State *L);
int lPlayersIterInit(lua_State *L);

int lIterNext(lua_State *L);
