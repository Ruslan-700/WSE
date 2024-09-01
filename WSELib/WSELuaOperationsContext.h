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
	public:
		lua_State *luaState;
		int callTriggerOpcode;
		std::unordered_map<std::string, gameOperation*> operationMap;
		std::vector<gameConstTable> gameConstTables;
		std::unordered_map<std::string, int> gvarMap;
		int operationHookLuaRefs[WSE_MAX_NUM_OPERATIONS];
		std::unordered_map<rgl::string, int> operationMgrHookLuaRefs; //hookScript
		std::chrono::steady_clock::time_point tStart;
		std::vector<bool> lua_call_cfResults;
		int luaContext = 0;

	public:
		WSELuaOperationsContext();
		void hookOperation(lua_State *L, int opcode, int lRef);
		void hookScript(lua_State *L, int script_no, int lRef);
		bool OnOperationExecute(int lRef, int num_operands, int *operand_types, __int64 *operand_values, bool *continue_loop, bool &setRetVal, long long &retVal);
		void *OnOperationJumptableExecute(wb::operation *operation, int *operand_types, __int64 *operand_values, bool *continue_loop, __int64 *locals, int context_flags);
		bool OnOperationMgrExecute(wb::operation_manager *operation_manager, int& num_parameters, __int64* parameters);

	protected:
		bool luaStateIsReady = false;

	protected:
		virtual void OnLoad();
		virtual void OnUnload();
		virtual void OnEvent(WSEContext *sender, WSEEvent evt, void *data);

		inline void initLua();

		void applyFlagListToOperationMap(std::unordered_map<std::string, std::vector<std::string>*> &flagLists, std::string listName, unsigned short flag, std::string opFile);
		void loadGameConstants(const std::string &dir);
		void loadGlobalVars();
		inline void loadOperations();
		inline void doMainScript();
};