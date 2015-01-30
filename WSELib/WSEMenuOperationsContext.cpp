#include "WSEMenuOperationsContext.h"

#include "WSE.h"
#include "warband.h"

int MenuCreateNew(WSEMenuOperationsContext *context)
{
#if defined WARBAND
	int flags, script_no, script_param;
	std::string text, mesh_name;
	
	context->ExtractString(text);
	context->ExtractString(mesh_name);
	context->ExtractValue(flags);
	context->ExtractValue(script_no, -1);
	context->ExtractValue(script_param);

	WSEDynamicArray<wb::menu> menus(&warband->menus, &warband->num_menus);
	wb::menu menu;

	menu.id.format("auto_menu_%d", warband->num_menus);
	menu.flags = flags;
	menu.text = text;
	menu.mesh_name = mesh_name;

	if (script_no >= 0 && script_no < warband->script_manager.num_scripts)
	{
		menu.operations.id.format("Menu [%d] %s", warband->num_menus, menu.id.c_str());
		menu.operations.operations = rgl::_new_vec<wb::operation>(1);
		menu.operations.num_operations = 1;
		menu.operations.operations[0].opcode = 1;
		menu.operations.operations[0].operands[0] = script_no;
		menu.operations.operations[0].operands[1] = warband->num_menus;
		menu.operations.operations[0].operands[2] = script_param;
		menu.operations.operations[0].num_operands = 3;
	}

	menus.PushBack(menu);

	return warband->num_menus - 1;
#else
	return -1;
#endif
}

void MenuAddItem(WSEMenuOperationsContext *context)
{
#if defined WARBAND
	int menu_no, conditions_script_no, consequences_script_no, script_param;
	std::string text;
	
	context->ExtractMenuNo(menu_no);
	context->ExtractString(text);
	context->ExtractValue(conditions_script_no, -1);
	context->ExtractValue(consequences_script_no, -1);
	context->ExtractValue(script_param);
	
	wb::menu *menu = &warband->menus[menu_no];
	WSEDynamicArray<wb::menu_item> menu_items(&menu->menu_items, &menu->num_menu_items);
	wb::menu_item menu_item;

	menu_item.id.format("%s_item_%d", menu->id.c_str(), menu->num_menu_items);
	menu_item.text = text;

	if (conditions_script_no >= 0 && conditions_script_no < warband->script_manager.num_scripts)
	{
		menu_item.conditions.id.format("Menu [%d] %s Menu Item [%d] %s Conditions", menu_no, menu->id.c_str(), menu->num_menu_items, menu_item.id.c_str());
		menu_item.conditions.operations = rgl::_new_vec<wb::operation>(1);
		menu_item.conditions.num_operations = 1;
		menu_item.conditions.operations[0].opcode = 1;
		menu_item.conditions.operations[0].operands[0] = conditions_script_no;
		menu_item.conditions.operations[0].operands[1] = menu_no;
		menu_item.conditions.operations[0].operands[2] = script_param;
		menu_item.conditions.operations[0].num_operands = 3;
	}

	if (consequences_script_no >= 0 && consequences_script_no < warband->script_manager.num_scripts)
	{
		menu_item.consequences.id.format("Menu [%d] %s Menu Item [%d] %s Consequences", menu_no, menu->id.c_str(), menu->num_menu_items, menu_item.id.c_str());
		menu_item.consequences.operations = rgl::_new_vec<wb::operation>(1);
		menu_item.consequences.num_operations = 1;
		menu_item.consequences.operations[0].opcode = 1;
		menu_item.consequences.operations[0].operands[0] = consequences_script_no;
		menu_item.consequences.operations[0].operands[1] = menu_no;
		menu_item.consequences.operations[0].operands[2] = script_param;
		menu_item.consequences.operations[0].num_operands = 3;
	}

	menu_items.PushBack(menu_item);
#endif
}

void MenuClear(WSEMenuOperationsContext *context)
{
#if defined WARBAND
	int menu_no;
	
	context->ExtractMenuNo(menu_no);

	wb::menu *menu = &warband->menus[menu_no];
	WSEDynamicArray<wb::menu_item> menu_items(&menu->menu_items, &menu->num_menu_items);
	
	menu_items.Clear();
#endif
}

void MenuClearGenerated(WSEMenuOperationsContext *context)
{
#if defined WARBAND
	WSEDynamicArray<wb::menu> menus(&warband->menus, &warband->num_menus);
	
	menus.Resize(context->m_num_base_menus);
#endif
}

WSEMenuOperationsContext::WSEMenuOperationsContext() : WSEOperationContext("menu", 4800, 4899)
{
}

void WSEMenuOperationsContext::OnLoad()
{
	m_num_base_menus = warband->num_menus;

	RegisterOperation("menu_create_new", MenuCreateNew, Client, Lhs, 2, 6,
		"Creates a dynamic menu and stores its id into <0>. <4> (-1 for no script) will be called with params 1 = menu_no, 2 = <5> when the operations block is executed",
		"destination", "text", "mesh_name", "flags", "script_no", "script_param");

	RegisterOperation("menu_add_item", MenuAddItem, Client, None, 2, 5,
		"Adds a new menu item to <0>. <2> and <3> (-1 for no script) will be called with params 1 = <0>, 2 = <4> when the conditions/consequences blocks are executed",
		"menu_no", "text", "conditions_script_no", "consequences_script_no", "script_param");

	RegisterOperation("menu_clear_items", MenuClear, Client, None, 1, 1,
		"Removes all menu items from <0>",
		"menu_no");

	RegisterOperation("menu_clear_generated", MenuClearGenerated, Client, None, 0, 0,
		"Removes all dynamic menus");
}
