#include "WSEDebugOperationsContext.h"

#include "WSE.h"
#include "warband.h"

#if defined DEBUG
void Debug(WSEDebugOperationsContext *context)
{
}

void SendRemoteScript(WSEDebugOperationsContext *context)
{
	int script_no;
	int params[16];
	int num_params = 0;

	context->ExtractScriptNo(script_no);

	while (context->HasMoreOperands())
	{
		context->ExtractValue(params[num_params++]);
	}

	WSE->Network.SendRemoteScript(&warband->script_manager.scripts[script_no], num_params, params);
}
#endif

WSEDebugOperationsContext::WSEDebugOperationsContext() : WSEOperationContext("debug", 4000, 4099)
{
}

void WSEDebugOperationsContext::OnLoad()
{
#if defined DEBUG
	ReplaceOperation(4095, "debug_break", Debug, Both, Undocumented, 0, 16,
		"Do not use ;d",
		"value", "value", "value", "value", "value", "value", "value", "value", "value", "value", "value", "value", "value", "value", "value", "value");

	ReplaceOperation(4096, "send_remote_script", SendRemoteScript, Client, Undocumented, 0, 16,
		"Do not use ;d",
		"value", "value", "value", "value", "value", "value", "value", "value", "value", "value", "value", "value", "value", "value", "value", "value");
#endif
}
