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
		lua_State *luaState;
		int callTriggerOpcode;
		std::unordered_map<std::string, gameOperation> operationMap;

	public:
		WSELuaOperationsContext();
		void printLastError(const char *fileName = NULL);

	protected:
		virtual void OnLoad();
		virtual void OnUnload();


		inline void initLua();

		void applyFlagListToOperationMap(std::unordered_map<std::string, std::vector<std::string>*> &flagLists, std::string listName, unsigned short flag, std::string opFile);
		inline void loadOperations();
		inline void addGameConstants();
		inline void initLGameTable();
		inline void doMainScript();
};