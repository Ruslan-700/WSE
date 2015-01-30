#include "menu.h"

#include "warband.h"

using namespace wb;

menu::menu()
{
	menu_items = nullptr;
	num_menu_items = 0;
}

menu::menu(const menu &obj)
{
	menu_items = nullptr;
	*this = obj;
}

menu::~menu()
{
	rgl::_delete_vec(menu_items);
}

menu &menu::operator =(const menu &obj)
{
	rgl::_delete_vec(menu_items);
	text = obj.text;
	id = obj.id;
	flags = obj.flags;
	mesh_name = obj.mesh_name;
	operations = obj.operations;
	num_menu_items = obj.num_menu_items;

	if (num_menu_items)
	{
		menu_items = rgl::_new_vec<menu_item>(num_menu_items);

		for (int i = 0; i < num_menu_items; ++i)
		{
			menu_items[i] = obj.menu_items[i];
		}
	}
	else
	{
		menu_items = nullptr;
	}

	return *this;
}
