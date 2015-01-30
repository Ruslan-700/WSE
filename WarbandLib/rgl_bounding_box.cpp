#include "rgl_bounding_box.h"

using namespace rgl;

bounding_box::bounding_box()
{
	this->min = rgl::vector4(0.0f);
	this->max = rgl::vector4(0.0f);
	this->center = rgl::vector4(0.0f);
	this->diameter = 0.0f;
}
