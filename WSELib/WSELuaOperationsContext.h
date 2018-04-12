#pragma once

#include <chrono>
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
	public: //I really should write getters but meh...
		lua_State *luaState;
		int callTriggerOpcode;
		std::unordered_map<std::string, gameOperation> operationMap;
		std::vector<gameConstTable> gameConstTables;
		int operationHookLuaRefs[WSE_MAX_NUM_OPERATIONS];
		std::chrono::steady_clock::time_point tStart;
		bool dontFailMsCall;

	public:
		WSELuaOperationsContext();
		void hookOperation(lua_State *L, int opcode, int lRef);
		bool unhookOperation(lua_State *L, int opcode);
		bool OnOperationExecute(int lRef, int num_operands, int *operand_types, __int64 *operand_values, bool *continue_loop, bool &setRetVal, long long &retVal);
		void *OnOperationJumptableExecute(wb::operation *operation, int *operand_types, __int64 *operand_values, bool *continue_loop, __int64 *locals, int context_flags);

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