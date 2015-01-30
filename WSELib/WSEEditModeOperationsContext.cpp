#include "WSEEditModeOperationsContext.h"

#include "WSE.h"
#include "warband.h"

bool EditModeInEditObjectsMode(WSEEditModeOperationsContext *context)
{
	wb::game *game = warband->cur_game;
	
	return game->edit_mode_active && game->edit_mode_mode == 0;
}

int EditModeGetNumSelectedMissionObjects(WSEEditModeOperationsContext *context)
{
	wb::game *game = warband->cur_game;

	if (!game->edit_mode_active)
		return -1;

	return game->edit_mode_selected_mission_object_nos.size();
}

int EditModeGetSelectedMissionObject(WSEEditModeOperationsContext *context)
{
	int index;

	context->ExtractValue(index);

	wb::game *game = warband->cur_game;
	
	if (!game->edit_mode_active || index < 0 || index >= game->edit_mode_selected_mission_object_nos.size())
		return -1;

	return game->edit_mode_selected_mission_object_nos[index];
}

void EditModeSelectMissionObject(WSEEditModeOperationsContext *context)
{
	int mission_object_no;

	context->ExtractMissionObjectNo(mission_object_no);

	wb::game *game = warband->cur_game;

	if (!game->edit_mode_active || game->edit_mode_selected_mission_object_nos.contains(mission_object_no))
		return;

	game->edit_mode_selected_mission_object_nos.push_back(mission_object_no);
	game->edit_mode_selection_changed = true;
}

void EditModeDeselectMissionObject(WSEEditModeOperationsContext *context)
{
	int mission_object_no;

	context->ExtractMissionObjectNo(mission_object_no);

	wb::game *game = warband->cur_game;

	if (!game->edit_mode_active)
		return;

	int index = game->edit_mode_selected_mission_object_nos.find(mission_object_no);

	if (index < 0)
		return;

	if (game->edit_mode_highlighted_mission_object_no == index)
		game->edit_mode_highlighted_mission_object_no = -1;
	else if (game->edit_mode_highlighted_mission_object_no > index)
		game->edit_mode_highlighted_mission_object_no--;

	game->edit_mode_selected_mission_object_nos.remove_at(index);
	game->edit_mode_selection_changed = true;
}

int EditModeGetHighlightedMissionObject(WSEEditModeOperationsContext *context)
{
	wb::game *game = warband->cur_game;

	if (!game->edit_mode_active || game->edit_mode_highlighted_mission_object_no < 0 || game->edit_mode_highlighted_mission_object_no >= game->edit_mode_selected_mission_object_nos.size())
		return -1;

	return game->edit_mode_selected_mission_object_nos[game->edit_mode_highlighted_mission_object_no];
}

void EditModeSetHighlightedMissionObject(WSEEditModeOperationsContext *context)
{
	int mission_object_no;

	if (context->HasMoreOperands())
		context->ExtractMissionObjectNo(mission_object_no);
	else
		mission_object_no = -1;

	wb::game *game = warband->cur_game;

	if (!game->edit_mode_active || game->edit_mode_highlighted_mission_object_no == mission_object_no)
		return;

	game->edit_mode_highlighted_mission_object_no = mission_object_no;
	game->edit_mode_selection_changed = true;
}

void EditModeSetEnabled(WSEEditModeOperationsContext *context)
{
	bool value;

	context->ExtractBoolean(value);

	warband->config.enable_edit_mode = value ? 1 : 0;
}

WSEEditModeOperationsContext::WSEEditModeOperationsContext() : WSEOperationContext("edit_mode", 4600, 4699)
{
}

void WSEEditModeOperationsContext::OnLoad()
{
	RegisterOperation("edit_mode_in_edit_objects_mode", EditModeInEditObjectsMode, Client, Cf, 0, 0,
		"Fails if the game is not in edit objects mode");

	RegisterOperation("edit_mode_get_num_selected_prop_instances", EditModeGetNumSelectedMissionObjects, Client, Lhs, 1, 1,
		"Stores the number of selected prop instances into <destination>",
		"destination");

	RegisterOperation("edit_mode_get_selected_prop_instance", EditModeGetSelectedMissionObject, Client, Lhs, 2, 2,
		"Stores the <1>th selected prop instance into instance no into <0>",
		"destination", "index");

	RegisterOperation("edit_mode_select_prop_instance", EditModeSelectMissionObject, Client, None, 1, 1,
		"Stores the <1>th selected prop instance into instance no into <0>",
		"prop_instance_no");

	RegisterOperation("edit_mode_deselect_prop_instance", EditModeDeselectMissionObject, Client, None, 1, 1,
		"Stores the <1>th selected prop instance into instance no into <0>",
		"prop_instance_no");

	RegisterOperation("edit_mode_get_highlighted_prop_instance", EditModeGetHighlightedMissionObject, Client, Lhs, 1, 1,
		"Stores the highlighted prop instance into <0>",
		"destination");

	RegisterOperation("edit_mode_set_highlighted_prop_instance", EditModeSetHighlightedMissionObject, Client, None, 0, 1,
		"Stores the <1>th selected prop instance into instance no into <0>",
		"prop_instance_no");

	RegisterOperation("edit_mode_set_enabled", EditModeSetEnabled, Client, None, 1, 1,
		"Enables or disables edit mode",
		"value");
}
