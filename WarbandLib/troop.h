#pragma once

#include "rgl.h"
#include "basics.h"
#include "face_generator.h"
#include "item.h"

#define NUM_TROOP_ATTRIBUTES 4
#define NUM_TROOP_PROFICIENCIES 7

namespace wb
{
	struct troop
	{
		bool renamed;
		rgl::string name;
		rgl::string name_plural;
		rgl::string image_mesh_name;
		rgl::string id;
		unsigned __int64 flags;
		int reserved[4];
		int type;
		int faction_no;
		int upgrade_troop_nos[2];
		int attributes[4];
		int skills[6];
		float proficiencies[7];
		int experience;
		float health;
		int skill_points;
		int attribute_points;
		int proficiency_points;
		int level;
		int class_no;
		inventory inventory;
		int gold;
		int site_no_and_entry;
		int spawned;
		face_keys face_keys_1;
		face_keys face_keys_2;
		note_manager notes;
		item equipped_items[NUM_ITEM_SLOTS];
		slot_manager slots;

		void update_health();
		int get_skill(int skill_no) const;
		void set_skill(int skill_no, int value);
		int get_attribute(int attribute) const;
		void set_attribute(int attribute, int value);
	};
}
