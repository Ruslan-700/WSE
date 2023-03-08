#include "WSEInputOperationsContext.h"

#include "WSE.h"
#include "warband.h"

bool KeyIsDown(WSEInputOperationsContext *context)
{
	int key;
	bool bypass_console_check;

	context->ExtractBoundedValue(key, 0, RGL_NUM_KEYS);
	context->ExtractBoolean(bypass_console_check);

	return (bypass_console_check || !warband->cur_game || !warband->cur_game->console_mode) && warband->input_manager.is_key_down(key);
}

bool KeyClicked(WSEInputOperationsContext *context)
{
	int key;
	bool bypass_console_check;

	context->ExtractBoundedValue(key, 0, RGL_NUM_KEYS);
	context->ExtractBoolean(bypass_console_check);

	return (bypass_console_check || !warband->cur_game || !warband->cur_game->console_mode) && warband->input_manager.is_key_clicked(key);
}

bool GameKeyIsDown(WSEInputOperationsContext *context)
{
	int game_key_no;
	bool bypass_console_check;

	context->ExtractBoundedValue(game_key_no, 0, NUM_GAME_KEYS);
	context->ExtractBoolean(bypass_console_check);

	return (bypass_console_check || !warband->cur_game || !warband->cur_game->console_mode) && warband->game_key_manager.is_key_down(game_key_no);
}

bool GameKeyClicked(WSEInputOperationsContext *context)
{
	int game_key_no;
	bool bypass_console_check;

	context->ExtractBoundedValue(game_key_no, 0, NUM_GAME_KEYS);
	context->ExtractBoolean(bypass_console_check);

	return (bypass_console_check || !warband->cur_game || !warband->cur_game->console_mode) && warband->game_key_manager.is_key_clicked(game_key_no);
}

int GameKeyGetKey(WSECoreOperationsContext *context)
{
	int game_key_no;

	context->ExtractBoundedValue(game_key_no, 0, NUM_GAME_KEYS);

	return warband->game_key_manager.game_keys[game_key_no].key;
}

bool KeyReleased(WSEInputOperationsContext *context)
{
	int key;
	bool bypass_console_check;
	
	context->ExtractBoundedValue(key, 0, RGL_NUM_KEYS);
	context->ExtractBoolean(bypass_console_check);

	return (bypass_console_check || !warband->cur_game || !warband->cur_game->console_mode) && warband->input_manager.is_key_released(key);
}

bool GameKeyReleased(WSEInputOperationsContext *context)
{
	int game_key_no;
	bool bypass_console_check;
	
	context->ExtractBoundedValue(game_key_no, 0, NUM_GAME_KEYS);
	context->ExtractBoolean(bypass_console_check);

	return (bypass_console_check || !warband->cur_game || !warband->cur_game->console_mode) && warband->game_key_manager.is_key_released(game_key_no);
}

WSEInputOperationsContext::WSEInputOperationsContext() : WSEOperationContext("input", 3100, 3199)
{
}

void WSEInputOperationsContext::OnLoad()
{
	ReplaceOperation(70, "key_is_down", KeyIsDown, Client, Cf | Fail, 1, 2,
		"Fails if <0> is not currently down",
		"key", "bypass_console_check");

	ReplaceOperation(71, "key_clicked", KeyClicked, Client, Cf | Fail, 1, 2,
		"Fails if <0> is not clicked on the specific frame",
		"key", "bypass_console_check");

	ReplaceOperation(72, "game_key_is_down", GameKeyIsDown, Client, Cf | Fail, 1, 2,
		"Fails if <0> is not currently down",
		"game_key_no", "bypass_console_check");

	ReplaceOperation(73, "game_key_clicked", GameKeyClicked, Client, Cf | Fail, 1, 2,
		"Fails if <0> is not clicked on the specific frame",
		"game_key_no", "bypass_console_check");

	RegisterOperation("game_key_get_key", GameKeyGetKey, Both, Lhs, 2, 2,
		"Stores the key mapped to <1> into <0>",
		"destination", "game_key_no");

	RegisterOperation("key_released", KeyReleased, Client, Cf | Fail, 1, 2,
		"Fails if <0> wasn't released in the current frame",
		"key", "bypass_console_check");

	RegisterOperation("game_key_released", GameKeyReleased, Client, Cf | Fail, 1, 2,
		"Fails if <0> wasn't released in the current frame",
		"game_key_no", "bypass_console_check");
}
