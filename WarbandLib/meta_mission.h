#pragma once

#include "rgl.h"

namespace wb
{
	struct meta_mission
	{
		int encountered_parties[2];
		int event_no;
		int u1;
		int flags;
		int type;
		int u2;
		int battle_advantage;
		bool ended;
		bool end_committed;
		int main_party_role;
		int enemy_strength;
		int mission_type;
		int u3;
		bool u4;
		bool u5;
		int u6;
		int result_flags;
		bool in_event;
		int prev_mission_type;
		int u7[60];
		int enemy_side;
		int conversation_party_no;
		int conversation_troop_no;
		int enemies_present;
		int u8;
		int u9;
		int u10;
		int mission_template_no;
		int site_no;
		int entry_point_no;
		int u11;
		int battle_flags;
		int num_upgradeable_troops;
		float u12;
		float u13;
		int u14[64];
		int u15;
		int u16;
		int battle_advantage_delta;
		int u17[16];
		int sequence_no;
		int passage_menu_no;
		bool party_battle_mode;
		int u18;
	};
}
