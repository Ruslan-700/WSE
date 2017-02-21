#pragma once

#include <unordered_map>
#include <lua.hpp>

#include "WSEOperationContext.h"

struct gameOperation
{
	unsigned int opcode;
	unsigned short flags;
};

class WSELuaOperationsContext : public WSEOperationContext
{
public:
	WSELuaOperationsContext();
	std::unordered_map<std::string, gameOperation> operationMap;

protected:
	lua_State *luaState;

	virtual void OnLoad();
	virtual void OnUnload();

	void printLastError();

	inline void initLua();

	void applyFlagListToOperationMap(std::unordered_map<std::string, std::vector<std::string>*> &flagLists, std::string listName, unsigned short flag, std::string opFile);
	inline void loadOperations();
	inline void initLGameTable();
	inline void doMainScript();

	//void luaCall();
	//void luaPush_str();
	//void luaPop();
};

static int lGameTableHandler(lua_State *L);