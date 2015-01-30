#include "WSEOptionsOperationsContext.h"

#include "WSE.h"
#include "warband.h"

WSEOptionsOperationsContext::WSEOptionsOperationsContext() : WSEOperationContext("options", 4300, 4399)
{
}

int OptionsGetBattleSize(WSEOptionsOperationsContext *context)
{
	return round_half_up(warband->config.battle_size * 120) + 30;
}

void OptionsSetBattleSize(WSEOptionsOperationsContext *context)
{
	int value;

	context->ExtractValue(value);

	if (value < 30)
		value = 30;

	if (value > 2000)
		value = 2000;

	warband->config.battle_size = ((value - 30) / 120.0f);
}

int OptionsGetVerboseCasualties(WSEOptionsOperationsContext *context)
{
	return warband->config.verbose_casualties;
}

void OptionsSetVerboseCasualties(WSEOptionsOperationsContext *context)
{
	bool value;

	context->ExtractBoolean(value);

	warband->config.verbose_casualties = value ? 1 : 0;
}

int OptionsGetCheatMode(WSECoreOperationsContext *context)
{
	return warband->config.cheat_mode;
}

void OptionsSetCheatMode(WSECoreOperationsContext *context)
{
	bool mode;

	context->ExtractBoolean(mode);

	warband->config.cheat_mode = mode ? 1 : 0;
}

int OptionsGetRealisticHeadshots(WSECoreOperationsContext *context)
{
	return warband->config.realistic_headshots;
}

void OptionsSetRealisticHeadshots(WSECoreOperationsContext *context)
{
	bool value;

	context->ExtractBoolean(value);

	warband->config.realistic_headshots = value ? 1 : 0;
}

void WSEOptionsOperationsContext::OnLoad()
{
	RegisterOperation("options_get_battle_size", OptionsGetBattleSize, Client, Lhs, 1, 1,
		"Stores battle size into <0>",
		"destination");

	RegisterOperation("options_set_battle_size", OptionsSetBattleSize, Client, None, 1, 1,
		"Sets batte size to <0> (must be between 30 and 2000)",
		"value");

	RegisterOperation("options_get_verbose_casualties", OptionsGetVerboseCasualties, Client, Lhs, 1, 1,
		"Stores verbose casualties enabled/disabled into <0>",
		"destination");

	RegisterOperation("options_set_verbose_casualties", OptionsSetVerboseCasualties, Client, None, 1, 1,
		"Enables or disables verbose casualties",
		"value");

	RegisterOperation("options_get_cheat_mode", OptionsGetCheatMode, Client, Lhs, 1, 1,
		"Stores cheat mode enabled/disabled into <0>",
		"destination");

	RegisterOperation("options_set_cheat_mode", OptionsSetCheatMode, Client, None, 1, 1,
		"Enables or disables cheat mode",
		"value");

	RegisterOperation("options_get_realistic_headshots", OptionsGetRealisticHeadshots, Client, Lhs, 1, 1,
		"Stores \"realistic\" headshots enabled/disabled into <0>",
		"destination");

	RegisterOperation("options_set_realistic_headshots", OptionsSetRealisticHeadshots, Client, None, 1, 1,
		"Enables or disables \"realistic\" headshots",
		"value");
}
