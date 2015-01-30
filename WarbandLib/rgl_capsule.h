#pragma once

#include "rgl_body_part.h"
#include "rgl_vector4.h"

namespace rgl
{
	struct capsule : body_part
	{
		vector4 center;
		vector4 direction;
		vector4 base_center;
		vector4 base_direction;
		float radius;
		
		capsule(float radius, const vector4 &center, const vector4 &direction);
		~capsule();
		int get_type() const;
		const char *get_name() const;
		void read(const io_stream &stream);
		void write(const io_stream &stream);
		body_part *copy();
		void unknown_1(int);
		void scale(const vector4 &scale);
		void transform(const matrix &matrix);
		void set_entity_no(const int &id);
		const vector4 &get_b_min();
		const vector4 &get_b_max();
		vector4 &store_min(vector4 &v);
		vector4 &store_max(vector4 &v);
		void update_min_max();
		void set_flags(const unsigned int &flag, const bool &update_physics_body);
		void set_flag(const unsigned int &flag, const bool &update_physics_body);
		void clear_flag(const unsigned int &flag, const bool &update_physics_body);
	};
}
