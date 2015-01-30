#include "WSEPositionOperationsContext.h"

#include <cmath>
#include "WSE.h"
#include "warband.h"

void PositionRotateX(WSEPositionOperationsContext *context)
{
	int preg, deg;
	bool use_global_axis;

	context->ExtractRegister(preg);
	context->ExtractValue(deg);
	context->ExtractBoolean(use_global_axis);

	float rad = DEG2RAD(deg);
	rgl::matrix *matrix = &warband->basic_game.position_registers[preg];
	
	if (use_global_axis)
	{
		matrix->rot.s.rotate_x(rad);
		matrix->rot.f.rotate_x(rad);
		matrix->rot.u.rotate_x(rad);
	}
	else
	{
		matrix->rotate_x(rad);
	}
}

void PositionRotateY(WSEPositionOperationsContext *context)
{
	int preg, deg;
	bool use_global_axis;

	context->ExtractRegister(preg);
	context->ExtractValue(deg);
	context->ExtractBoolean(use_global_axis);

	float rad = DEG2RAD(deg);
	rgl::matrix *matrix = &warband->basic_game.position_registers[preg];
	
	if (use_global_axis)
	{
		matrix->rot.s.rotate_y(rad);
		matrix->rot.f.rotate_y(rad);
		matrix->rot.u.rotate_y(rad);
	}
	else
	{
		matrix->rotate_y(rad);
	}
}

void PositionRotateZ(WSEPositionOperationsContext *context)
{
	int preg, deg;
	bool use_global_axis;

	context->ExtractRegister(preg);
	context->ExtractValue(deg);
	context->ExtractBoolean(use_global_axis);

	float rad = DEG2RAD(deg);
	rgl::matrix *matrix = &warband->basic_game.position_registers[preg];
	
	if (use_global_axis)
	{
		matrix->rot.s.rotate_z(rad);
		matrix->rot.f.rotate_z(rad);
		matrix->rot.u.rotate_z(rad);
	}
	else
	{
		matrix->rotate_z(rad);
	}
}

void PositionRotateXFloating(WSEPositionOperationsContext *context)
{
	int preg;
	float deg;
	bool use_global_axis;

	context->ExtractRegister(preg);
	context->ExtractFixedPoint(deg);
	context->ExtractBoolean(use_global_axis);

	float rad = DEG2RAD(deg);
	rgl::matrix *matrix = &warband->basic_game.position_registers[preg];
	
	if (use_global_axis)
	{
		matrix->rot.s.rotate_x(rad);
		matrix->rot.f.rotate_x(rad);
		matrix->rot.u.rotate_x(rad);
	}
	else
	{
		matrix->rotate_x(rad);
	}
}

void PositionRotateYFloating(WSEPositionOperationsContext *context)
{
	int preg;
	float deg;
	bool use_global_axis;

	context->ExtractRegister(preg);
	context->ExtractFixedPoint(deg);
	context->ExtractBoolean(use_global_axis);

	float rad = DEG2RAD(deg);
	rgl::matrix *matrix = &warband->basic_game.position_registers[preg];
	
	if (use_global_axis)
	{
		matrix->rot.s.rotate_y(rad);
		matrix->rot.f.rotate_y(rad);
		matrix->rot.u.rotate_y(rad);
	}
	else
	{
		matrix->rotate_y(rad);
	}
}

void PositionRotateZFloating(WSEPositionOperationsContext *context)
{
	int preg;
	float deg;
	bool use_global_axis;

	context->ExtractRegister(preg);
	context->ExtractFixedPoint(deg);
	context->ExtractBoolean(use_global_axis);

	float rad = DEG2RAD(deg);
	rgl::matrix *matrix = &warband->basic_game.position_registers[preg];

	if (use_global_axis)
	{
		matrix->rot.s.rotate_z(rad);
		matrix->rot.f.rotate_z(rad);
		matrix->rot.u.rotate_z(rad);
	}
	else
	{
		matrix->rotate_z(rad);
	}
}

int PositionGetVectorToPosition(WSEPositionOperationsContext *context)
{
	int preg1, preg2, preg3;
	
	context->ExtractRegister(preg1);
	context->ExtractRegister(preg2);
	context->ExtractRegister(preg3);

	rgl::matrix *dest = &warband->basic_game.position_registers[preg1];
	rgl::vector4 pos1 = warband->basic_game.position_registers[preg2].o;
	rgl::vector4 pos2 = warband->basic_game.position_registers[preg3].o;
	rgl::vector4 v = pos2 - pos1;
	
	dest->initialize();
	dest->o = pos1;
	dest->rotate_z(-std::atan2(v.x, v.y));
	dest->rotate_x(std::atan2(v.z, std::sqrt(v.x * v.x + v.y * v.y)));

	return (int)(v.length() * warband->basic_game.fixed_point_multiplier);
}

void PositionAlignToGround(WSEPositionOperationsContext *context)
{
	int preg;
	bool pointUp;
	bool changeHeight;
	
	context->ExtractRegister(preg);
	context->ExtractBoolean(pointUp);
	context->ExtractBoolean(changeHeight);

	rgl::matrix *dest = &warband->basic_game.position_registers[preg];
	float height;
	rgl::quad *quad;

	warband->cur_mission->mission_scene->get_ground_data(dest->o, height, &quad);

	if (pointUp)
	{
		dest->rot.u = rgl::vector4(0.0f, 0.0f, 1.0f);
		dest->rot.f = quad ? quad->normal : rgl::vector4(0.0f, 1.0f, 0.0f);
		dest->rot.f.normalize();
		dest->rot.s.cross(dest->rot.f, dest->rot.u);
		dest->rot.s.normalize();
		dest->rot.u.cross(dest->rot.s, dest->rot.f);
	}
	else
	{
		dest->rot.f = rgl::vector4(0.0f, 1.0f, 0.0f);
		dest->rot.u = quad ? quad->normal : rgl::vector4(0.0f, 0.0f, 1.0f);
		dest->rot.u.normalize();
		dest->rot.s.cross(dest->rot.f, dest->rot.u);
		dest->rot.s.normalize();
		dest->rot.f.cross(dest->rot.s, dest->rot.u);
	}

	if (changeHeight)
		dest->o.z = height;
}

WSEPositionOperationsContext::WSEPositionOperationsContext() : WSEOperationContext("position", 4100, 4199)
{
}

void WSEPositionOperationsContext::OnLoad()
{
	ReplaceOperation(723, "position_rotate_x", PositionRotateX, Both, None, 2, 3,
		"Rotates <0> around the x-axis by <1> degrees",
		"position_register", "angle", "use_global_axis");

	ReplaceOperation(724, "position_rotate_y", PositionRotateY, Both, None, 2, 3,
		"Rotates <0> around the y-axis by <1> degrees",
		"position_register", "angle", "use_global_axis");

	ReplaceOperation(725, "position_rotate_z", PositionRotateZ, Both, None, 2, 3,
		"Rotates <0> around the z-axis by <1> degrees",
		"position_register", "angle", "use_global_axis");

	ReplaceOperation(738, "position_rotate_x_floating", PositionRotateXFloating, Both, None, 2, 3,
		"Rotates <0> around the x-axis by <1> degrees",
		"position_register", "angle_fixed_point", "use_global_axis");

	ReplaceOperation(739, "position_rotate_y_floating", PositionRotateYFloating, Both, None, 2, 3,
		"Rotates <0> around the y-axis by <1> degrees",
		"position_register", "angle_fixed_point", "use_global_axis");

	RegisterOperation("position_rotate_z_floating", PositionRotateZFloating, Both, None, 2, 3,
		"Rotates <0> around the z-axis by <1> degrees",
		"position_register", "angle_fixed_point", "use_global_axis");
	
	RegisterOperation("position_get_vector_to_position", PositionGetVectorToPosition, Both, Lhs, 4, 4,
		"Stores the vector from <2> to <3> into <1> and its length into <0>",
		"destination_fixed_point", "dest_position_register", "position_register_1", "position_register_2");
	
	RegisterOperation("position_align_to_ground", PositionAlignToGround, Both, None, 1, 3,
		"Aligns <0> to the ground (or to the ground normal if <1> is set)",
		"position_register", "point_up", "set_z_to_ground_level");
}
