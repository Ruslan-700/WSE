#include "WSEInputOperationsContext.h"

#include "WSE.h"
#include "warband.h"

int GameKeyGetKey(WSECoreOperationsContext *context)
{
	int game_key_no;

	context->ExtractBoundedValue(game_key_no, 0, NUM_GAME_KEYS);

	return warband->game_key_manager.game_keys[game_key_no].key;
}

bool KeyReleased(WSEInputOperationsContext *context)
{
	int key;
	
	context->ExtractBoundedValue(key, 0, 256);

	return warband->input_manager.is_key_released(key);
}

bool GameKeyReleased(WSEInputOperationsContext *context)
{
	int game_key_no;
	
	context->ExtractBoundedValue(game_key_no, 0, NUM_GAME_KEYS);

	return (!warband->cur_game || !warband->cur_game->console_mode) && warband->game_key_manager.is_key_released(game_key_no);
}

WSEInputOperationsContext::WSEInputOperationsContext() : WSEOperationContext("input", 3100, 3199)
{
}

void WSEInputOperationsContext::OnLoad()
{
	RegisterOperation("game_key_get_key", GameKeyGetKey, Both, Lhs, 2, 2,
		"Stores the key mapped to <1> into <0>",
		"destination", "game_key_no");

	RegisterOperation("key_released", KeyReleased, Client, Cf | Fail, 1, 1,
		"Fails if <0> wasn't released in the current frame",
		"key");

	RegisterOperation("game_key_released", GameKeyReleased, Client, Cf | Fail, 1, 1,
		"Fails if <0> wasn't released in the current frame",
		"game_key_no");
}
