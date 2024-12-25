#pragma once

#include <lua.hpp>
#include "WSE.h"

enum triggerPart: int
{
	condition,
	consequence
};

int lGameExecOperationHandler(lua_State *L);

int lGetRegHandler(lua_State *L);
int lSetRegHandler(lua_State *L);

int lGetGvarHandler(lua_State *L);
int lSetGvarHandler(lua_State *L);

int lGetScriptNo(lua_State *L);
int lGetCurTemplateNo(lua_State *L);
int lGetCurTemplateId(lua_State *L);
int lGetNumTemplates(lua_State *L);
int lGetTemplateId(lua_State *L);

int lAddTrigger(lua_State *L);
int lRemoveTrigger(lua_State *L);
int lGetNumTriggers(lua_State *L);

int lAddItemTrigger(lua_State *L);
int lAddPropTrigger(lua_State *L);

int lAddPrsnt(lua_State *L);
int lRemovePrsnt(lua_State *L);
int lAddPsys(lua_State *L);
int lRemovePsys(lua_State *L);

int lPartiesIterInit(lua_State *L);
int lAgentsIterInit(lua_State *L);
int lPropInstIterInit(lua_State *L);
int lPlayersIterInit(lua_State *L);

int lPrint(lua_State *L);

int lHookOperation(lua_State *L);
int lUnhookOperation(lua_State *L);
int lHookScript(lua_State *L);

int lGetTime(lua_State *L);
int lFailMsCall(lua_State *L);

int lPrintStack(lua_State *L);