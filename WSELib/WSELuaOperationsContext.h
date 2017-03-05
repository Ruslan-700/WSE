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
	lua_State *luaState;
	void printLastError();
	int callTriggerOpcode;

protected:

	virtual void OnLoad();
	virtual void OnUnload();


	inline void initLua();

	void applyFlagListToOperationMap(std::unordered_map<std::string, std::vector<std::string>*> &flagLists, std::string listName, unsigned short flag, std::string opFile);
	inline void loadOperations();
	inline void initLGameTable();
	inline void doMainScript();

	//void luaCall();
	//void luaPush_str();
	//void luaPop();
};