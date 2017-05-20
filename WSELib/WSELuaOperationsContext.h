#pragma once

#include <unordered_map>
#include <lua.hpp>

#include "WSEOperationContext.h"


struct gameOperation
{
	unsigned int opcode;
	unsigned short flags;
};

struct gameConst
{
	std::string name;
	float val;
};

struct gameConstTable
{
	std::string name;
	std::vector<gameConst> constants;
};

class WSELuaOperationsContext : public WSEOperationContext
{
	public:
		lua_State *luaState;
		int callTriggerOpcode;
		std::unordered_map<std::string, gameOperation> operationMap;
		std::vector<gameConstTable> gameConstTables;
		int operationHooks[WSE_MAX_NUM_OPERATIONS];

	public:
		WSELuaOperationsContext();
		bool OnOperationExecute(int lRef, int num_operands, int *operand_types, __int64 *operand_values, bool *continue_loop, bool &setRetVal, long long &retVal);

	protected:
		bool luaStateIsReady = false;

	protected:
		virtual void OnLoad();
		virtual void OnUnload();
		virtual void OnEvent(WSEContext *sender, WSEEvent evt, void *data);

		inline void initLua();

		void applyFlagListToOperationMap(std::unordered_map<std::string, std::vector<std::string>*> &flagLists, std::string listName, unsigned short flag, std::string opFile);
		void loadGameConstants(const std::string &dir);
		inline void loadOperations();
		inline void doMainScript();
};