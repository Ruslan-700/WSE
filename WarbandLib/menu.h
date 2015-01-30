#pragma once

#include "rgl.h"
#include "operation.h"

namespace wb
{
	struct menu_item
	{
		rgl::string text;
		rgl::string door_name;
		rgl::string id;
		operation_manager conditions;
		operation_manager consequences;
	};

	struct menu
	{
		rgl::string text;
		rgl::string id;
		unsigned __int64 flags;
		rgl::string mesh_name;
		operation_manager operations;
		int num_menu_items;
		menu_item *menu_items;
		
		menu();
		~menu();
		menu(const menu &obj);
		menu &operator =(const menu &obj);
	};
}
