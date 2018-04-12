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

int lAddItemTrigger(lua_State *L);

#if defined WARBAND
int lAddPrsnt(lua_State *L);
int lRemovePrsnt(lua_State *L);
#endif

int lPartiesIterInit(lua_State *L);
int lAgentsIterInit(lua_State *L);
int lPropInstIterInit(lua_State *L);
int lPlayersIterInit(lua_State *L);

int lPrint(lua_State *L);

int lHookOperation(lua_State *L);
int lUnhookOperation(lua_State *L);

int lGetTime(lua_State *L);
int lFailMsCall(lua_State *L);