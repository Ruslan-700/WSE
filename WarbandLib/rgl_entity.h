#pragma once

#include "stl_vector.h"
#include "rgl_body_part.h"
#include "rgl_io_stream.h"
#include "rgl_matrix.h"
#include "rgl_mesh.h"
#include "rgl_bounding_box.h"
#include "rgl_meta_mesh.h"
#include "rgl_pool.h"
#include "rgl_render_context.h"
#include "rgl_scene.h"
#include "rgl_vector4.h"
#include "Physics\Dynamics\Entity\hkpRigidBody.h"

namespace rgl
{
	struct render_context;
	struct scene;
	struct scene_grid_coordinate;

	enum entity_type
	{
		et_mesh      = 1,
		et_collision = 3, // deprecated
		et_static    = 5,
		et_dynamic   = 7, // deprecated
		et_strategic = 9,
		et_skeleton  = 11, // deprecated
	};

	enum entity_flags
	{
		ef_cast_shadow_smooth  = 0x1,
		ef_cast_shadow_precise = 0x2,
		ef_lod_mask            = 0xF0,
	};

	struct entity
	{
		scene *scene;
		int no;
		int object_no;
		int object_type;
		scene_grid_coordinate *grid_coord;
		matrix prev_position;
		matrix position;
		body_part *collision_object;
		vector4 b_min;
		vector4 b_max;
#if defined WARBAND
		float u1;
		bounding_box bounding_box;
		stl::vector<meta_mesh *> meta_meshes;
		stl::vector<mesh *> meshes;
#endif
		stl::vector<body_part *> static_body_parts;
		hkpRigidBody *havok_body;
		bool havok_body_moved;
		bool dynamic_physics;
#if defined WARBAND
		mesh *shadow_1_mesh;
		mesh *shadow_2_mesh;
#endif
		bool visible;
		bool moved_in_this_frame;
		bool moved_in_prev_frame;
		float level_of_detail;
		unsigned int flags;
		vector4 scale;
		entity *u2;

		virtual ~entity();
		virtual int get_type() const;
		virtual const string &get_name(const string &name = "") const;
		virtual void update_body_bounding_box();
#if defined WARBAND
		virtual void update_mesh_bounding_box();
		virtual double get_radius() const;
		virtual double set_radius(const float &radius, bool update_body) const;
#endif
		virtual void run(float f);
		virtual void move_with_parent();
		virtual void set_level_of_detail(float *lod);
#if defined WARBAND
		virtual void render(render_context *rc, void *ri);
		virtual void select_level_of_detail(const vector4 &off);
#endif
		virtual bool is_moveable();
		virtual bool unknown_1();
		virtual const vector4 &get_position();
		virtual const vector4 &get_max();
		virtual const vector4 &get_min();
		virtual bool has_moved();
		virtual void read(const io_stream &stream);
		virtual void unknown_2();
#if defined WARBAND
		virtual void set_shadow_type(unsigned int type);
		virtual void clear_meshes();
#endif
		virtual void set_mesh_position(const matrix &pos);
		virtual void set_body_position(const matrix &pos);
		virtual void set_mesh_position_and_scale(const matrix &pos, const vector4 &scale);
		virtual void set_body_position_and_scale(const matrix &pos, const vector4 &scale);
	};
}
