#pragma once

#include "rgl.h"
#include "simple_trigger.h"

namespace wb
{
	enum scene_prop_flags
	{
		sokf_type_container = 0x5,
		sokf_type_ai_limiter = 0x8,
		sokf_type_barrier = 0x9,
		sokf_type_barrier_leave = 0xA,
		sokf_type_ladder = 0xB,
		sokf_type_barrier3d = 0xC,
		sokf_type_player_limiter = 0xD,
		sokf_type_ai_limiter3d = 0xE,
		sokf_add_fire = 0x100,
		sokf_add_smoke = 0x200,
		sokf_add_light = 0x400,
		sokf_show_hit_point_bar = 0x800,
		sokf_place_at_origin = 0x1000,
		sokf_dynamic = 0x2000,
		sokf_invisible = 0x4000,
		sokf_destructible = 0x8000,
		sokf_moveable = 0x10000,
		sokf_face_player = 0x20000,
		sokf_dynamic_physics = 0x40000,
		sokf_missiles_not_attached = 0x80000,
		sokf_enforce_shadows = 0x100000,
		sokf_dont_move_agent_over = 0x200000,
		sokf_handle_as_flora = 0x1000000,
		sokf_static_movement = 0x2000000,
		sokf_weapon_knock_back_collision = 0x10000000, // WSE
	};

	struct scene_prop
	{
		rgl::string id;
		unsigned __int64 flags;
		int hit_points;
		float u1[10];
#if defined WARBAND
		rgl::meta_mesh *meta_mesh;
#endif
		rgl::body *body_object;
		rgl::body_part *body_part;
		rgl::vector4 b_center;
		float b_diameter;
		simple_trigger_manager simple_triggers;
	};
}
