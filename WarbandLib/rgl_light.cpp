#include "rgl_light.h"

using namespace rgl;

light::light()
{
	this->type = lt_point;
	this->diffuse_color = vector4(0.0f, 0.0f, 0.0f);
	this->specular_color = vector4(0.0f, 0.0f, 0.0f);
	this->position = vector4(0.0f, 0.0f, 0.0f);
	this->direction = vector4(0.0f, 1.0f, 0.0f);
	this->local_position = vector4(0.0f, 0.0f, 0.0f);
	this->range = 0.0f;
	this->attenuation_0 = 0.0f;
	this->attenuation_1 = 1.0f;
	this->attenuation_2 = 0.0f;
	this->theta = 0.0f;
	this->phi = 0.0f;
	this->flicker_magnitude = 0.0f;
	this->flicker_interval = 0.0f;
	this->flags = 0;
	this->u1 = -1;
}

void light::update_range()
{
	if (this->type == lt_point)
		this->range = (this->diffuse_color.x + this->diffuse_color.y + this->diffuse_color.z) * 10.0f;
}
