#pragma once

#include <map>
#include <string>
#include "WSEContext.h"
#include "warband.h"

#define WSE_MAX_NUM_OPERATIONS 5000
#define WSE_FIRST_WARBAND_OPCODE 31
#define WSE_LAST_WARBAND_OPCODE 2392
#define MAX_NUM_NESTED_LOOPS 256

class WSEScriptingContext;

typedef void (__cdecl *OperationCallback)(void *context);
typedef int (__cdecl *LhsOperationCallback)(void *context);
typedef bool (__cdecl *CfOperationCallback)(void *context);

enum WSEOperationType
{
	Operation,
	LhsOperation,
	CfOperation,
	NoOperation,
};

enum WSEOperationFlags
{
	None = 0x0,
	Lhs = 0x1,
	Cf = 0x2,
	Disable = 0x4,
	Fail = 0x8,
	BreakNetwork = 0x10,
	Control = 0x20,
	Undocumented = 0x40,
};

enum WSEOperationTarget
{
	Both,
	Client,
	Server,
};

struct WSEOperationDescriptor
{
	bool m_core;
	WSEContext *m_context;
	void *m_callback;
	WSEOperationType m_type;
	WSEOperationTarget m_target;
	unsigned short m_flags;
	unsigned int m_opcode;
	short m_min_operands;
	short m_max_operands;
	std::string m_name;
	std::string m_description;
	std::string m_operands[16];
};

class WSEScriptingLoop
{
public:
	int statement_no;
	int value;
	wb::mission_grid_iterator mission_grid_iterator;
	std::map<std::string, std::string>::const_iterator dict_it;
	std::map<std::string, std::string>::const_iterator dict_end_it;
};

class WSEScriptingLoopManager
{
public:
	WSEScriptingLoop loops[MAX_NUM_NESTED_LOOPS];
	int num_loops;

public:
	void Clear();
	int Size();
	void AddLoop(int statement_no, int value);
	void EndLoop();
	void EndLoop(int index);
	WSEScriptingLoop *GetLoop();
	WSEScriptingLoop *GetLoop(int index);
};

class WSEScriptingContext : public WSEContext
{
public:
	WSEScriptingContext();

protected:
	virtual void OnLoad();
	virtual void OnUnload();
	virtual void OnEvent(WSEContext *sender, WSEEvent evt);

public:
	bool ExecuteStatementBlock(wb::operation_manager *operation_manager, int context_flags, int depth, int &num_parameters, __int64 *parameters);
	void StartLoop(wb::operation_manager *operation_manager, __int64 *local_variables, WSEScriptingLoopManager &loop_manager, int &statement_no);
	void EndLoop(wb::operation_manager *operation_manager, __int64 *local_variables, WSEScriptingLoopManager &loop_manager, int &statement_no);
	bool CanLoop(wb::operation_manager *operation_manager, __int64 *local_variables, WSEScriptingLoopManager &loop_manager);
	void AddOperation(WSEContext *context, void *callback, WSEOperationTarget target, unsigned short flags, short min_operands, short max_operands, unsigned int opcode, const std::string &name, const std::string &description, std::string *operands);
	void UnregisterOperations(WSEContext *context);
	int GetTriggerParam(int index) const;
	void SetTriggerParam(int index, int value);

private:
	void DumpOperationsHeader();

private:
	bool OnOperationExecute(wb::operation *operation, int *operand_types, __int64 *operand_values, bool *continue_loop, __int64 *locals, int context_flags);

public:
	wb::operation_manager *m_cur_statement_block;

private:
	WSEOperationDescriptor *m_descriptors[WSE_MAX_NUM_OPERATIONS];
	int m_trigger_params[16];
	bool m_allow_unset_script_params;
};
