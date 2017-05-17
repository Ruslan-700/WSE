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
		void printLastError(const char *fileName = NULL, HANDLE hFile = INVALID_HANDLE_VALUE);

	protected:
		bool luaStateIsReady = false;

	protected:
		virtual void OnLoad();
		virtual void OnUnload();
		virtual void OnEvent(WSEContext *sender, WSEEvent evt, void *data);

		inline void initLua();

		void applyFlagListToOperationMap(std::unordered_map<std::string, std::vector<std::string>*> &flagLists, std::string listName, unsigned short flag, std::string opFile);
		inline void loadOperations();
		inline void addGameConstants(const std::string &dir, bool first);
		inline void initLGameTable();
		inline void doMainScript();
};