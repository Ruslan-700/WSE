#include "WSEMathOperationsContext.h"

#include "WSE.h"

int RightShift(WSEMathOperationsContext *context)
{
	int value, shift;

	context->ExtractLhsValue(value);
	context->ExtractValue(shift);

	return value >> shift;
}

int StoreRightShift(WSEMathOperationsContext *context)
{
	int value, shift;

	context->ExtractValue(value);
	context->ExtractValue(shift);

	return value >> shift;
}

int LogicalRightShift(WSEMathOperationsContext *context)
{
	int value, shift;

	context->ExtractLhsValue(value);
	context->ExtractValue(shift);

	return (unsigned int)value >> shift;
}

int StoreLogicalRightShift(WSEMathOperationsContext *context)
{
	int value, shift;

	context->ExtractValue(value);
	context->ExtractValue(shift);

	return (unsigned int)value >> shift;
}

int LeftShift(WSEMathOperationsContext *context)
{
	int value, shift;

	context->ExtractLhsValue(value);
	context->ExtractValue(shift);

	return value << shift;
}

int StoreLeftShift(WSEMathOperationsContext *context)
{
	int value, shift;

	context->ExtractValue(value);
	context->ExtractValue(shift);

	return value << shift;
}

int Xor(WSEMathOperationsContext *context)
{
	int value1, value2;

	context->ExtractLhsValue(value1);
	context->ExtractValue(value2);

	return value1 ^ value2;
}

int StoreXor(WSEMathOperationsContext *context)
{
	int value1, value2;

	context->ExtractValue(value1);
	context->ExtractValue(value2);

	return value1 ^ value2;
}

int Not(WSEMathOperationsContext *context)
{
	int value;

	context->ExtractLhsValue(value);

	return ~value;
}

int StoreNot(WSEMathOperationsContext *context)
{
	int value;

	context->ExtractValue(value);

	return ~value;
}

WSEMathOperationsContext::WSEMathOperationsContext() : WSEOperationContext("math", 2800, 2899)
{
}

void WSEMathOperationsContext::OnLoad()
{
	RegisterOperation("val_shr", RightShift, Both, Lhs, 2, 2,
		"Performs an arithmetic right bit shift by <1> on <0>",
		"value", "shift");

	RegisterOperation("store_shr", StoreRightShift, Both, Lhs, 3, 3,
		"Performs an arithmetic right bit shift by <2> on <1> and stores the result into <0>",
		"destination", "value", "shift");

	RegisterOperation("val_lshr", LogicalRightShift, Both, Lhs, 2, 2,
		"Performs a logical right bit shift by <1> on <0>",
		"value", "shift");

	RegisterOperation("store_lshr", StoreLogicalRightShift, Both, Lhs, 3, 3,
		"Performs a logical right bit shift by <2> on <1> and stores the result into <0>",
		"destination", "value", "shift");

	RegisterOperation("val_shl", LeftShift, Both, Lhs, 2, 2,
		"Performs a left bit shift by <1> on <0>",
		"value", "shift");

	RegisterOperation("store_shl", StoreLeftShift, Both, Lhs, 3, 3,
		"Performs a left bit shift by <2> on <1> and stores the result into <0>",
		"destination", "value", "shift");

	RegisterOperation("val_xor", Xor, Both, Lhs, 2, 2,
		"Performs a bitwise exclusive or between <0> and <1>",
		"value1", "value2");

	RegisterOperation("store_xor", StoreXor, Both, Lhs, 3, 3,
		"Performs a bitwise exclusive or between <1> and <2> and stores the result into <0>",
		"destination", "value1", "value2");

	RegisterOperation("val_not", Not, Both, Lhs, 1, 1,
		"Performs a bitwise complement on <0>",
		"value");

	RegisterOperation("store_not", StoreNot, Both, Lhs, 2, 2,
		"Performs a bitwise complement on <1> and stores the result into <0>",
		"destination", "value");
}
