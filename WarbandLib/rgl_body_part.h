#pragma once

#include "rgl_matrix.h"
#include "rgl_vector4.h"
#include "rgl_io_stream.h"

namespace rgl
{
	enum body_part_flags
	{
		bf_no_collision       = 0x2,
		bf_invisible          = 0x4,
		bf_indestructible     = 0x8,
		bf_terrain_quad       = 0x10,
		bf_barrier_ai         = 0x100,
		bf_tree               = 0x200,
		bf_dusty              = 0x4000,
		bf_barrier_2d         = 0x10000,
		bf_dynamic_physics    = 0x20000,
		bf_moveable           = 0x40000,
		bf_ladder             = 0x80000,
		bf_barrier_3d         = 0x100000,
	};

	enum body_type
	{
		bt_sphere    = 0,
		bt_capsule   = 1,
		bt_quad      = 2,
		bt_composite = 3,
		bt_manifold  = 4,
	};

	struct intersection_point
	{
		vector4 rotation;
		vector4 position;
		float depth;
	};

	struct body_part
	{
		int scene_manifold_face_no;
		unsigned int flags;
		int entity_no;
		int min_grid_cells[2];
		int max_grid_cells[2];
		body_part *parent_body_part;
		vector4 b_min;
		vector4 b_max;

		virtual ~body_part();
		virtual int get_type() const = 0;
		virtual const char *get_name() const = 0;
		virtual void read(const io_stream &stream) = 0;
		virtual void write(const io_stream &stream) = 0;
		virtual body_part *copy() = 0;
		virtual void unknown_1(int) = 0;
		virtual void scale(const vector4 &scale) = 0;
		virtual void transform(const matrix &matrix) = 0;
		virtual void set_entity_no(const int &id) = 0;
		virtual const vector4 &get_b_min() = 0;
		virtual const vector4 &get_b_max() = 0;
		virtual vector4 &store_min(vector4 &v) = 0;
		virtual vector4 &store_max(vector4 &v) = 0;
		virtual void update_min_max() = 0;
		virtual void set_flags(const unsigned int &flag, const bool &update_physics_body) = 0;
		virtual void set_flag(const unsigned int &flag, const bool &update_physics_body) = 0;
		virtual void clear_flag(const unsigned int &flag, const bool &update_physics_body) = 0;
	};
}
