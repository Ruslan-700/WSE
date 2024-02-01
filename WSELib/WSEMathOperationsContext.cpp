#include "WSEMathOperationsContext.h"

#include "WSE.h"

__int64 RightShift(WSEMathOperationsContext *context)
{
	__int64 value, shift;

	context->ExtractLhsBigValue(value);
	context->ExtractBigValue(shift);

	return value >> shift;
}

__int64 StoreRightShift(WSEMathOperationsContext *context)
{
	__int64 value, shift;

	context->ExtractBigValue(value);
	context->ExtractBigValue(shift);

	return value >> shift;
}

__int64 LogicalRightShift(WSEMathOperationsContext *context)
{
	__int64 value, shift;

	context->ExtractLhsBigValue(value);
	context->ExtractBigValue(shift);

	return (unsigned __int64)value >> shift;
}

__int64 StoreLogicalRightShift(WSEMathOperationsContext *context)
{
	__int64 value, shift;

	context->ExtractBigValue(value);
	context->ExtractBigValue(shift);

	return (unsigned __int64)value >> shift;
}

__int64 LeftShift(WSEMathOperationsContext *context)
{
	__int64 value, shift;

	context->ExtractLhsBigValue(value);
	context->ExtractBigValue(shift);

	return value << shift;
}

__int64 StoreLeftShift(WSEMathOperationsContext *context)
{
	__int64 value, shift;

	context->ExtractBigValue(value);
	context->ExtractBigValue(shift);

	return value << shift;
}

__int64 Xor(WSEMathOperationsContext *context)
{
	__int64 value1, value2;

	context->ExtractLhsBigValue(value1);
	context->ExtractBigValue(value2);

	return value1 ^ value2;
}

__int64 StoreXor(WSEMathOperationsContext *context)
{
	__int64 value1, value2;

	context->ExtractBigValue(value1);
	context->ExtractBigValue(value2);

	return value1 ^ value2;
}

__int64 Not(WSEMathOperationsContext *context)
{
	__int64 value;

	context->ExtractLhsBigValue(value);

	return ~value;
}

__int64 StoreNot(WSEMathOperationsContext *context)
{
	__int64 value;

	context->ExtractBigValue(value);

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
