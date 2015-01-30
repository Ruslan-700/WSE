#include "WSEFloatingPointOperationsContext.h"

#include <cmath>
#include <random>
#include "WSE.h"
#include "warband.h"

void FLd(WSEFloatingPointOperationsContext *context)
{
	int freg;
	float value;

	context->ExtractRegister(freg);
	context->ExtractFixedPoint(value);

	context->m_float_registers[freg] = value;
}

void FLdStr(WSEFloatingPointOperationsContext *context)
{
	int freg;
	rgl::string value;

	context->ExtractRegister(freg);
	context->ExtractString(value);

	context->m_float_registers[freg] = (float)atof(value.c_str());
}

void FLdPosX(WSEFloatingPointOperationsContext *context)
{
	int freg, preg;
	rgl::string value;

	context->ExtractRegister(freg);
	context->ExtractRegister(preg);

	context->m_float_registers[freg] = warband->basic_game.position_registers[preg].o.x;
}

void FLdPosY(WSEFloatingPointOperationsContext *context)
{
	int freg, preg;
	rgl::string value;

	context->ExtractRegister(freg);
	context->ExtractRegister(preg);

	context->m_float_registers[freg] = warband->basic_game.position_registers[preg].o.y;
}

void FLdPosZ(WSEFloatingPointOperationsContext *context)
{
	int freg, preg;
	rgl::string value;

	context->ExtractRegister(freg);
	context->ExtractRegister(preg);

	context->m_float_registers[freg] = warband->basic_game.position_registers[preg].o.z;
}

int FSt(WSEFloatingPointOperationsContext *context)
{
	int freg;
	
	context->ExtractRegister(freg);

	return (int)(context->m_float_registers[freg] * warband->basic_game.fixed_point_multiplier);
}

void FCpy(WSEFloatingPointOperationsContext *context)
{
	int freg1, freg2;
	
	context->ExtractRegister(freg1);
	context->ExtractRegister(freg2);

	context->m_float_registers[freg1] = context->m_float_registers[freg2];
}

bool FEq(WSEFloatingPointOperationsContext *context)
{
	int freg1, freg2;
	
	context->ExtractRegister(freg1);
	context->ExtractRegister(freg2);

	return fabs(context->m_float_registers[freg1] - context->m_float_registers[freg2]) <= DBL_EPSILON;
}

bool FGt(WSEFloatingPointOperationsContext *context)
{
	int freg1, freg2;
	
	context->ExtractRegister(freg1);
	context->ExtractRegister(freg2);

	return context->m_float_registers[freg1] > context->m_float_registers[freg2];
}

bool FLt(WSEFloatingPointOperationsContext *context)
{
	int freg1, freg2;
	
	context->ExtractRegister(freg1);
	context->ExtractRegister(freg2);
	
	return context->m_float_registers[freg1] < context->m_float_registers[freg2];
}

bool FGe(WSEFloatingPointOperationsContext *context)
{
	int freg1, freg2;
	
	context->ExtractRegister(freg1);
	context->ExtractRegister(freg2);

	float val1 = context->m_float_registers[freg1];
	float val2 = context->m_float_registers[freg2];
	
	return val1 > val2 || fabs(val1 - val2) <= DBL_EPSILON;
}

bool FLe(WSEFloatingPointOperationsContext *context)
{
	int freg1, freg2;
	
	context->ExtractRegister(freg1);
	context->ExtractRegister(freg2);

	float val1 = context->m_float_registers[freg1];
	float val2 = context->m_float_registers[freg2];
	
	return val1 < val2 || fabs(val1 - val2) <= DBL_EPSILON;
}

void FAdd(WSEFloatingPointOperationsContext *context)
{
	int freg1, freg2, freg3;
	
	context->ExtractRegister(freg1);
	context->ExtractRegister(freg2);
	context->ExtractRegister(freg3);

	context->m_float_registers[freg1] = context->m_float_registers[freg2] + context->m_float_registers[freg3];
}

void FSub(WSEFloatingPointOperationsContext *context)
{
	int freg1, freg2, freg3;
	
	context->ExtractRegister(freg1);
	context->ExtractRegister(freg2);
	context->ExtractRegister(freg3);

	context->m_float_registers[freg1] = context->m_float_registers[freg2] - context->m_float_registers[freg3];
}

void FMul(WSEFloatingPointOperationsContext *context)
{
	int freg1, freg2, freg3;
	
	context->ExtractRegister(freg1);
	context->ExtractRegister(freg2);
	context->ExtractRegister(freg3);

	context->m_float_registers[freg1] = context->m_float_registers[freg2] * context->m_float_registers[freg3];
}

void FDiv(WSEFloatingPointOperationsContext *context)
{
	int freg1, freg2, freg3;
	
	context->ExtractRegister(freg1);
	context->ExtractRegister(freg2);
	context->ExtractRegister(freg3);

	context->m_float_registers[freg1] = context->m_float_registers[freg2] / context->m_float_registers[freg3];
}

void FMin(WSEFloatingPointOperationsContext *context)
{
	int freg1, freg2, freg3;
	
	context->ExtractRegister(freg1);
	context->ExtractRegister(freg2);
	context->ExtractRegister(freg3);
	
	float min = context->m_float_registers[freg3];
	float value = context->m_float_registers[freg2];

	context->m_float_registers[freg1] = value < min ? min : value;
}

void FMax(WSEFloatingPointOperationsContext *context)
{
	int freg1, freg2, freg3;
	
	context->ExtractRegister(freg1);
	context->ExtractRegister(freg2);
	context->ExtractRegister(freg3);

	float max = context->m_float_registers[freg3];
	float value = context->m_float_registers[freg2];

	context->m_float_registers[freg1] = value > max ? max : value;
}

void FClamp(WSEFloatingPointOperationsContext *context)
{
	int freg1, freg2, freg3, freg4;
	
	context->ExtractRegister(freg1);
	context->ExtractRegister(freg2);
	context->ExtractRegister(freg3);
	context->ExtractRegister(freg4);

	float min = context->m_float_registers[freg3];
	float max = context->m_float_registers[freg4];
	float value = context->m_float_registers[freg2];

	if (min > max)
		std::swap(min, max);

	context->m_float_registers[freg1] = value < min ? min : (value > max ? max : value);
}

void FSqrt(WSEFloatingPointOperationsContext *context)
{
	int freg1, freg2;
	
	context->ExtractRegister(freg1);
	context->ExtractRegister(freg2);

	context->m_float_registers[freg1] = sqrt(context->m_float_registers[freg2]);
}

void FAbs(WSEFloatingPointOperationsContext *context)
{
	int freg1, freg2;
	
	context->ExtractRegister(freg1);
	context->ExtractRegister(freg2);

	context->m_float_registers[freg1] = abs(context->m_float_registers[freg2]);
}

void FCeil(WSEFloatingPointOperationsContext *context)
{
	int freg1, freg2;
	
	context->ExtractRegister(freg1);
	context->ExtractRegister(freg2);

	context->m_float_registers[freg1] = ceil(context->m_float_registers[freg2]);
}

void FFloor(WSEFloatingPointOperationsContext *context)
{
	int freg1, freg2;
	
	context->ExtractRegister(freg1);
	context->ExtractRegister(freg2);

	context->m_float_registers[freg1] = floor(context->m_float_registers[freg2]);
}

void FExp(WSEFloatingPointOperationsContext *context)
{
	int freg1, freg2;
	
	context->ExtractRegister(freg1);
	context->ExtractRegister(freg2);

	context->m_float_registers[freg1] = exp(context->m_float_registers[freg2]);
}

void FPow(WSEFloatingPointOperationsContext *context)
{
	int freg1, freg2, freg3;
	
	context->ExtractRegister(freg1);
	context->ExtractRegister(freg2);
	context->ExtractRegister(freg3);

	context->m_float_registers[freg1] = pow(context->m_float_registers[freg2], context->m_float_registers[freg3]);
}

void FLn(WSEFloatingPointOperationsContext *context)
{
	int freg1, freg2;
	
	context->ExtractRegister(freg1);
	context->ExtractRegister(freg2);

	context->m_float_registers[freg1] = log(context->m_float_registers[freg2]);
}

void FLog(WSEFloatingPointOperationsContext *context)
{
	int freg1, freg2, base;
	
	context->ExtractRegister(freg1);
	context->ExtractRegister(freg2);
	context->ExtractValue(base, 10);

	context->m_float_registers[freg1] = log10(context->m_float_registers[freg2]) / log10((float)base);
}

void FMod(WSEFloatingPointOperationsContext *context)
{
	int freg1, freg2, freg3;
	
	context->ExtractRegister(freg1);
	context->ExtractRegister(freg2);
	context->ExtractRegister(freg3);

	context->m_float_registers[freg1] = fmod(context->m_float_registers[freg2], context->m_float_registers[freg3]);
}

void FSin(WSEFloatingPointOperationsContext *context)
{
	int freg1, freg2;
	
	context->ExtractRegister(freg1);
	context->ExtractRegister(freg2);

	context->m_float_registers[freg1] = sin(context->m_float_registers[freg2]);
}

void FCos(WSEFloatingPointOperationsContext *context)
{
	int freg1, freg2;
	
	context->ExtractRegister(freg1);
	context->ExtractRegister(freg2);

	context->m_float_registers[freg1] = cos(context->m_float_registers[freg2]);
}

void FTan(WSEFloatingPointOperationsContext *context)
{
	int freg1, freg2;
	
	context->ExtractRegister(freg1);
	context->ExtractRegister(freg2);

	context->m_float_registers[freg1] = tan(context->m_float_registers[freg2]);
}

void FSinh(WSEFloatingPointOperationsContext *context)
{
	int freg1, freg2;
	
	context->ExtractRegister(freg1);
	context->ExtractRegister(freg2);

	context->m_float_registers[freg1] = sinh(context->m_float_registers[freg2]);
}

void FCosh(WSEFloatingPointOperationsContext *context)
{
	int freg1, freg2;
	
	context->ExtractRegister(freg1);
	context->ExtractRegister(freg2);

	context->m_float_registers[freg1] = cosh(context->m_float_registers[freg2]);
}

void FTanh(WSEFloatingPointOperationsContext *context)
{
	int freg1, freg2;
	
	context->ExtractRegister(freg1);
	context->ExtractRegister(freg2);

	context->m_float_registers[freg1] = tanh(context->m_float_registers[freg2]);
}

void FAsin(WSEFloatingPointOperationsContext *context)
{
	int freg1, freg2;
	
	context->ExtractRegister(freg1);
	context->ExtractRegister(freg2);

	context->m_float_registers[freg1] = asin(context->m_float_registers[freg2]);
}

void FAcos(WSEFloatingPointOperationsContext *context)
{
	int freg1, freg2;
	
	context->ExtractRegister(freg1);
	context->ExtractRegister(freg2);

	context->m_float_registers[freg1] = acos(context->m_float_registers[freg2]);
}

void FAtan(WSEFloatingPointOperationsContext *context)
{
	int freg1, freg2;
	
	context->ExtractRegister(freg1);
	context->ExtractRegister(freg2);

	context->m_float_registers[freg1] = atan(context->m_float_registers[freg2]);
}

void FAtan2(WSEFloatingPointOperationsContext *context)
{
	int freg1, freg2, freg3;
	
	context->ExtractRegister(freg1);
	context->ExtractRegister(freg2);
	context->ExtractRegister(freg3);

	context->m_float_registers[freg1] = atan2(context->m_float_registers[freg2], context->m_float_registers[freg3]);
}

void FEval(WSEFloatingPointOperationsContext *context)
{
	std::string expr;

	context->ExtractString(expr);
	context->m_parser.SetExpr(expr);

	try
	{
		context->m_parser.Eval();
	}
	catch (mu::Parser::exception_type &e)
	{
		context->ScriptError(e.GetMsg());
	}
}

WSEFloatingPointOperationsContext::WSEFloatingPointOperationsContext() : WSEOperationContext("floating_point", 4400, 4499)
{
}

mu::value_type mupDegToRad(mu::value_type value)
{
	return DEG2RAD(value);
}

mu::value_type mupRadToDeg(mu::value_type value)
{
	return RAD2DEG(value);
}

mu::value_type mupFloor(mu::value_type value)
{
	return floor(value);
}

mu::value_type mupCeil(mu::value_type value)
{
	return ceil(value);
}

mu::value_type mupFmod(mu::value_type x, mu::value_type y)
{
	return fmod(x, y);
}

mu::value_type mupAtan2(mu::value_type y, mu::value_type x)
{
	return atan2(y, x);
}

mu::value_type mupClamp(mu::value_type value, mu::value_type min, mu::value_type max)
{
	if (min > max)
		std::swap(min, max);

	return value < min ? min : (value > max ? max : value);
}

mu::value_type *muVarFactory(const char *name, void *user_data)
{
	if (name[0] == 'p' && name[1] == 'o' && name[2] == 's')
	{
		char *last = NULL;
		int reg = strtol(&name[3], &last, 10);

		if (reg >= 0 && reg < NUM_REGISTERS)
		{
			int next = last - name;
			
			if (strlen(name) - next >= 1 && (name[next + 1] == '\0' || name[next + 2] == '\0'))
			{
				if (name[next] == 's')
				{
					if (name[next + 1] == 'x')
					{
						return &warband->basic_game.position_registers[reg].rot.s.x;
					}
					else if (name[next + 1] == 'y')
					{
						return &warband->basic_game.position_registers[reg].rot.s.y;
					}
					else if (name[next + 1] == 'z')
					{
						return &warband->basic_game.position_registers[reg].rot.s.z;
					}
				}
				else if (name[next] == 'f')
				{
					if (name[next + 1] == 'x')
					{
						return &warband->basic_game.position_registers[reg].rot.f.x;
					}
					else if (name[next + 1] == 'y')
					{
						return &warband->basic_game.position_registers[reg].rot.f.y;
					}
					else if (name[next + 1] == 'z')
					{
						return &warband->basic_game.position_registers[reg].rot.f.z;
					}
				}
				else if (name[next] == 'u')
				{
					if (name[next + 1] == 'x')
					{
						return &warband->basic_game.position_registers[reg].rot.u.x;
					}
					else if (name[next + 1] == 'y')
					{
						return &warband->basic_game.position_registers[reg].rot.u.y;
					}
					else if (name[next + 1] == 'z')
					{
						return &warband->basic_game.position_registers[reg].rot.u.z;
					}
				}
				else if (name[next] == 'x')
				{
					return &warband->basic_game.position_registers[reg].o.x;
				}
				else if (name[next] == 'y')
				{
					return &warband->basic_game.position_registers[reg].o.y;
				}
				else if (name[next] == 'z')
				{
					return &warband->basic_game.position_registers[reg].o.z;
				}
			}
		}
	}
	
	char buf[256];

	sprintf_s(buf, "Unrecognized variable: %s", name);
	throw mu::Parser::exception_type(buf);
}

void WSEFloatingPointOperationsContext::OnLoad()
{
	m_parser.DefineFun("deg2rad", mupDegToRad);
	m_parser.DefineFun("rad2deg", mupRadToDeg);
	m_parser.DefineFun("floor", mupFloor);
	m_parser.DefineFun("ceil", mupCeil);
	m_parser.DefineFun("fmod", mupFmod);
	m_parser.DefineFun("atan2", mupAtan2);
	m_parser.DefineFun("clamp", mupClamp);

	for (int i = 0; i < NUM_REGISTERS; ++i)
	{
		char name[10];

		sprintf_s(name, "fp%d", i);
		m_parser.DefineVar(name, &m_float_registers[i]);
	}

	m_parser.SetVarFactory(muVarFactory);

	RegisterOperation("fld", FLd, Both, None, 2, 2,
		"Loads <1> into <0>",
		"fp_register", "value_fixed_point");

	RegisterOperation("fld_str", FLdStr, Both, None, 2, 2,
		"Parses <1> and loads it into <0>",
		"fp_register", "string");

	RegisterOperation("fld_pos_x", FLdPosX, Both, None, 2, 2,
		"Loads the x component of <1> into <0>",
		"fp_register", "position_register_no");

	RegisterOperation("fld_pos_y", FLdPosY, Both, None, 2, 2,
		"Loads the y component of <1> into <0>",
		"fp_register", "position_register_no");

	RegisterOperation("fld_pos_z", FLdPosZ, Both, None, 2, 2,
		"Loads the z component of <1> into <0>",
		"fp_register", "position_register_no");

	RegisterOperation("fst", FSt, Both, Lhs, 2, 2,
		"Stores <1> into <0>",
		"destination_fixed_point", "fp_register");

	RegisterOperation("fcpy", FCpy, Both, None, 2, 2,
		"Copies <1> into <0>",
		"fp_register_1", "fp_register_2");

	RegisterOperation("feq", FEq, Both, Cf, 3, 3,
		"Fails if <1> isn't approximately equal to <2>",
		"destination_fp_register", "fp_register_1", "fp_register_2");

	RegisterOperation("fgt", FGt, Both, Cf, 3, 3,
		"Fails if <1> isn't greater than <2>",
		"destination_fp_register", "fp_register_1", "fp_register_2");

	RegisterOperation("flt", FLt, Both, Cf, 3, 3,
		"Fails if <1> isn't less than <2>",
		"destination_fp_register", "fp_register_1", "fp_register_2");

	RegisterOperation("fge", FGe, Both, Cf, 3, 3,
		"Fails if <1> isn't greater or approximately equal to <2>",
		"destination_fp_register", "fp_register_1", "fp_register_2");

	RegisterOperation("fle", FLe, Both, Cf, 3, 3,
		"Fails if <1> isn't less or approximately equal to <2>",
		"destination_fp_register", "fp_register_1", "fp_register_2");

	RegisterOperation("fsub", FSub, Both, None, 3, 3,
		"Subtracts <2> from <1> and stores the result into <0>",
		"destination_fp_register", "fp_register_1", "fp_register_2");

	RegisterOperation("fmul", FMul, Both, None, 3, 3,
		"Multiplies <1> by <2> and stores the result into <0>",
		"destination_fp_register", "fp_register_1", "fp_register_2");

	RegisterOperation("fdiv", FDiv, Both, None, 3, 3,
		"Divides <1> by <2> and stores the result into <0>",
		"destination_fp_register", "fp_register_1", "fp_register_2");

	RegisterOperation("fmin", FMin, Both, None, 3, 3,
		"Stores the smaller value between <1> and <2> into <0>",
		"destination_fp_register", "fp_register_1", "fp_register_2");

	RegisterOperation("fmax", FMax, Both, None, 3, 3,
		"Stores the larger value between <1> and <2> into <0>",
		"destination_fp_register", "fp_register_1", "fp_register_2");

	RegisterOperation("fclamp", FClamp, Both, None, 4, 4,
		"Clamps <1> between <2> and <3> and stores the result into <0>",
		"destination_fp_register", "fp_register_1", "fp_register_2", "fp_register_3");
	
	RegisterOperation("fsqrt", FSqrt, Both, None, 2, 2,
		"Stores the square root of <1> into <0>",
		"destination_fp_register", "fp_register");
	
	RegisterOperation("fabs", FAbs, Both, None, 2, 2,
		"Stores the absolute value of <1> into <0>",
		"destination_fp_register", "fp_register");

	RegisterOperation("fceil", FCeil, Both, None, 2, 2,
		"Stores the ceiling of <1> into <0>",
		"destination_fp_register", "fp_register");

	RegisterOperation("ffloor", FFloor, Both, None, 2, 2,
		"Stores the floor of <1> into <0>",
		"destination_fp_register", "fp_register");

	RegisterOperation("fexp", FExp, Both, None, 2, 2,
		"Stores e raised to the power of <1> into <0>",
		"destination_fp_register", "fp_register");

	RegisterOperation("fpow", FPow, Both, None, 3, 3,
		"Stores <1> raised to the power of <2> into <0>",
		"destination_fp_register", "fp_register_1", "fp_register_2");

	RegisterOperation("fln", FLn, Both, None, 2, 2,
		"Stores the natural logarithm of <1> into <0>",
		"destination_fp_register", "fp_register");

	RegisterOperation("flog", FLog, Both, None, 2, 3,
		"Stores the base-<2> (default: base-10) logarithm of <1> into <0>",
		"destination_fp_register", "fp_register", "base");

	RegisterOperation("fmod", FMod, Both, None, 3, 3,
		"Stores the remainder of <1>/<2> into <0>",
		"destination_fp_register", "fp_register_1", "fp_register_2");

	RegisterOperation("fsin", FSin, Both, None, 2, 2,
		"Stores the sine of <1> into <0>",
		"destination_fp_register", "fp_register");

	RegisterOperation("fcos", FCos, Both, None, 2, 2,
		"Stores the cosine of <1> into <0>",
		"destination_fp_register", "fp_register");

	RegisterOperation("ftan", FTan, Both, None, 2, 2,
		"Stores the tangent of <1> into <0>",
		"destination_fp_register", "fp_register");

	RegisterOperation("fsinh", FSinh, Both, None, 2, 2,
		"Stores the hyperbolic sine of <1> into <0>",
		"destination_fp_register", "fp_register");

	RegisterOperation("fcosh", FCosh, Both, None, 2, 2,
		"Stores the hyperbolic cosine of <1> into <0>",
		"destination_fp_register", "fp_register");

	RegisterOperation("ftanh", FTanh, Both, None, 2, 2,
		"Stores the hyperbolic tangent of <1> into <0>",
		"destination_fp_register", "fp_register");

	RegisterOperation("fasin", FAsin, Both, None, 2, 2,
		"Stores the arc sine of <1> into <0>",
		"destination_fp_register", "fp_register");

	RegisterOperation("facos", FAcos, Both, None, 2, 2,
		"Stores the arc cosine of <1> into <0>",
		"destination_fp_register", "fp_register");

	RegisterOperation("fatan", FAtan, Both, None, 2, 2,
		"Stores the arc tangent of <1> into <0>",
		"destination_fp_register", "fp_register");

	RegisterOperation("fatan2", FAtan2, Both, None, 3, 3,
		"Stores the arc cosine of <1>/<2> into <0>",
		"destination_fp_register", "fp_register_1", "fp_register_2");

	RegisterOperation("feval", FEval, Both, None, 1, 1,
		"Evaluates <0>. See EVAL_README.txt in WSESDK for more information",
		"expression_string");
}

void WSEFloatingPointOperationsContext::OnUnload()
{
}
