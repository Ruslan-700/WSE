#pragma once

#include "rgl.h"
#include "item_kind.h"

#define NUM_INVENTORY_SLOTS 96
#define NUM_ITEM_SLOTS 10
#define AMMO_MASK 0xFF
#define HEALTH_MASK 0xFFFF
#define MODIFIER_MASK 0xFF000000
#define MODIFIER_SHIFT 24

namespace wb
{
	enum item_slot
	{
		is_item_0 = 0,
		is_item_1 = 1,
		is_item_2 = 2,
		is_item_3 = 3,
		is_head   = 4,
		is_body   = 5,
		is_foot   = 6,
		is_hand   = 7,
		is_horse  = 8,
		is_food   = 9,
	};

	struct item
	{
		int item_no;
		unsigned int item_flags;
		
		item();
		item(int item_no);
		item(int item_no, int item_modifier_no);
		void refill();
		int get_max_ammo() const;
		int get_ammo() const;
		void set_ammo(int ammo);
		int get_max_health() const;
		int get_health() const;
		void set_health(int health);
		int get_modifier() const;
		void set_modifier(int modifier);
		bool is_valid() const;
		item_kind *get_item_kind() const;
	};

	struct inventory
	{
		item slots[NUM_INVENTORY_SLOTS];
	};
}
