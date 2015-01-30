#pragma once

#include "rgl.h"
#include "game_window.h"
#include "item.h"

#define NUM_INVENTORY_PANELS 2

namespace wb
{
	struct inventory_window : game_window
	{
		void *inventory_menu_widgets[NUM_INVENTORY_PANELS];
		rgl::scrollable_widget *inventory_menu_scrollable_widgets[NUM_INVENTORY_PANELS];
		void *equipment_menu_widgets[NUM_INVENTORY_PANELS];
		rgl::widget *gold_widgets[NUM_INVENTORY_PANELS];
		rgl::widget *name_widgets[NUM_INVENTORY_PANELS];
		rgl::widget *hinge_widgets[4];
		void *inventory_slot_widgets[NUM_INVENTORY_PANELS * NUM_INVENTORY_SLOTS];
		unsigned int inventory_flags[NUM_INVENTORY_PANELS * NUM_INVENTORY_SLOTS];
		int inventory_prices[NUM_INVENTORY_PANELS * NUM_INVENTORY_SLOTS];
		int num_inventory_menus;
		int num_equipment_menus;
		rgl::popup_widget *popup_widget;
		void *equipment_slot_widgets[NUM_INVENTORY_PANELS * NUM_ITEM_SLOTS];
		unsigned int equipment_flags[NUM_INVENTORY_PANELS * NUM_ITEM_SLOTS];
		int equipment_prices[NUM_INVENTORY_PANELS * NUM_ITEM_SLOTS];
		rgl::widget *total_cost_widget;
		rgl::widget *character_1_description_widgets[4];
		rgl::widget *character_2_description_widgets[4];
		rgl::image_button_widget *character_1_image_button;
		rgl::image_button_widget *character_2_image_button;
		rgl::image_button_widget *character_1_shadow_image_button;
		rgl::image_button_widget *character_2_shadow_image_button;
		int total_cost;
		rgl::simple_button_widget *leave_button_widget;
		int u1;
		int dragging_item;
		item dragged_item;
		bool dragged_item_is_borrowed;
		int dragged_item_type;
		int dragged_item_source_grid_type;
		int merchant_question_result;
		int dump_items_question_result;
		int slaughter_food_question_result;
		bool is_main_party_troop;
		int character_1_rotation;
		int character_2_rotation;
		rgl::widget *equipment_panel_background_widget;
	};
}
