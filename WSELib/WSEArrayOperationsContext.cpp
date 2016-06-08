#include "WSEArrayOperationsContext.h"

#include "WSE.h"
#include "warband.h"
#include <string>
#include <stdio.h>

std::string IntVecToStr(std::vector <int> &vec)
{
	std::string res = "";

	for (size_t i = 0; i < vec.size(); i++){
		res += '[';
		res += std::to_string(vec[i]);
		res += ']';
	}

	return res;
}

void ExtractIndices(WSEArrayOperationsContext *context, std::vector<int> &indices)
{
	while (context->HasMoreOperands()){
		int curDim;
		context->ExtractValue(curDim);
		indices.push_back(curDim);
	}
}

int ArrayCreate(WSEArrayOperationsContext *context)
{
	int id;
	std::vector <int> dims;

	context->ExtractValue(id);
	ExtractIndices(context, dims);

	if ((dims.size() > 0) && (dims.size() <= MAX_DIMENSIONS)){
		if (id == type_id::num){
			WSEDynMultiArray2<int> *ptr = new WSEDynMultiArray2<int>(dims, (type_id)id);
			if (ptr->isValid())
				return WSE->Objects.AllocObject(context->m_type, (void*)ptr);
			delete ptr;
		}
		else if (id == type_id::str){
			WSEDynMultiArray2<std::string> *ptr = new WSEDynMultiArray2<std::string>(dims, (type_id)id);
			if (ptr->isValid())
				return WSE->Objects.AllocObject(context->m_type, (void*)ptr);
			delete ptr;
		}
		else if (id == type_id::pos){
			WSEDynMultiArray2<rgl::matrix> *ptr = new WSEDynMultiArray2<rgl::matrix>(dims, (type_id)id);
			if (ptr->isValid())
				return WSE->Objects.AllocObject(context->m_type, (void*)ptr);
			delete ptr;
		}
		else{
			context->ScriptError("invalid array type ID: " + std::to_string(id));
		}

	}

	context->ScriptError("invalid array dimensions: " + IntVecToStr(dims));
	return 0; //won't happen
}

void ArraySetInt(WSEArrayOperationsContext *context)
{
	int id, value;
	std::vector <int> indices;

	context->ExtractValue(id);
	context->ExtractValue(value);
	ExtractIndices(context, indices);

	WSEDynMultiArray2 <int> *array = (WSEDynMultiArray2 <int> *) context->GetArray(id);
	
	if (array->getTypeID() != type_id::num)
		context->ScriptError("array is not of type int");

	if (!array->setIndex(indices, value))
		context->ScriptError("failed to set index: " + IntVecToStr(indices));
}

void ArraySetStr(WSEArrayOperationsContext *context)
{
	int id;
	std::string value;
	std::vector <int> indices;

	context->ExtractValue(id);
	context->ExtractString(value);
	ExtractIndices(context, indices);

	WSEDynMultiArray2 <std::string> *array = (WSEDynMultiArray2 <std::string> *) context->GetArray(id);

	if (array->getTypeID() != type_id::str)
		context->ScriptError("array is not of type str");

	if (!array->setIndex(indices, value))
		context->ScriptError("failed to set index: " + IntVecToStr(indices));
}

void ArraySetPos(WSEArrayOperationsContext *context)
{
	int id, preg;
	rgl::matrix value;
	std::vector <int> indices;

	context->ExtractValue(id);

	context->ExtractRegister(preg);
	value = warband->basic_game.position_registers[preg];

	ExtractIndices(context, indices);

	WSEDynMultiArray2 <rgl::matrix> *array = (WSEDynMultiArray2 <rgl::matrix> *) context->GetArray(id);

	if (array->getTypeID() != type_id::pos)
		context->ScriptError("array is not of type pos");

	if (!array->setIndex(indices, value))
		context->ScriptError("failed to set index: " + IntVecToStr(indices));
}

int ArrayGetInt(WSEArrayOperationsContext *context)
{
	int id;
	std::vector <int> indices;

	context->ExtractValue(id);
	ExtractIndices(context, indices);

	WSEDynMultiArray2<int> *array = (WSEDynMultiArray2<int> *) context->GetArray(id);

	if (array->getTypeID() != type_id::num)
		context->ScriptError("array is not of type int");

	const int *valPtr = array->getIndex(indices);

	if (valPtr == NULL)
		context->ScriptError("failed to get index: " + IntVecToStr(indices));

	return *valPtr;
}

void ArrayGetStr(WSEArrayOperationsContext *context)
{
	int id, sreg;
	std::vector <int> indices;

	context->ExtractRegister(sreg);
	context->ExtractValue(id);
	ExtractIndices(context, indices);

	WSEDynMultiArray2<std::string> *array = (WSEDynMultiArray2<std::string> *) context->GetArray(id);

	if (array->getTypeID() != type_id::str)
		context->ScriptError("array is not of type str");

	const std::string *valPtr = array->getIndex(indices);

	if (valPtr == NULL){
		context->ScriptError("failed to get index: " + IntVecToStr(indices));
	}
	else{
		warband->basic_game.string_registers[sreg] = *valPtr;
	}
}

void ArrayGetPos(WSEArrayOperationsContext *context)
{
	int id, preg;
	std::vector <int> indices;

	context->ExtractRegister(preg);
	context->ExtractValue(id);
	ExtractIndices(context, indices);

	WSEDynMultiArray2<rgl::matrix> *array = (WSEDynMultiArray2<rgl::matrix> *) context->GetArray(id);

	if (array->getTypeID() != type_id::pos)
		context->ScriptError("array is not of type pos");

	const rgl::matrix *valPtr = array->getIndex(indices);

	if (valPtr == NULL){
		context->ScriptError("failed to get index: " + IntVecToStr(indices));
	}
	else{
		warband->basic_game.position_registers[preg] = *valPtr;
	}
}

void ArrayResize(WSEArrayOperationsContext *context)
{
	int id, size;

	context->ExtractValue(id);
	context->ExtractValue(size);

	WSEDynMultiArray2<int> *array = (WSEDynMultiArray2<int> *) context->GetArray(id);
	
	if (!array->resize((size_t)size)){
		context->ScriptError("Failed to resize array");
	}
}

int ArrayGetDimSize(WSEArrayOperationsContext *context)
{
	int id, index;

	context->ExtractValue(id);
	context->ExtractValue(index);

	WSEDynMultiArray2<int> *array = (WSEDynMultiArray2<int> *) context->GetArray(id);

	return array->getDimSize(index);
}

int ArrayGetDimCount(WSEArrayOperationsContext *context)
{
	int id;

	context->ExtractValue(id);

	WSEDynMultiArray2<int> *array = (WSEDynMultiArray2<int> *) context->GetArray(id);

	return array->getDimCount();
}

void *WSEArrayOperationsContext::GetArray(int id)
{
	void *array = WSE->Objects.FetchObject(m_type, id);

	if (!array)
		ScriptError("operand is not a valid array");

	return array;
}

WSEArrayOperationsContext::WSEArrayOperationsContext() : WSEOperationContext("array", 5000, 5099)
{

}

void WSEArrayOperationsContext::OnLoad()
{
	m_type = WSE->Objects.AllocType("array");

	//const std::string &name, void *callback, WSEOperationTarget target, unsigned int flags, short min_operands, short max_operands, std::string description, ...)
	RegisterOperation("array_create", ArrayCreate, Both, Lhs, 3, 16,
		"Creates an array object of <1> (0: Integer, 1: String, 2: Position) and stores its ID into <0>. You can specify up to 14 dimensions, from <2> to <15>.",
		"destination", "type", "Dim 0", "Dim 1", "Dim 2", "Dim 3", "Dim 4", "Dim 5", "Dim 6", "Dim 7", "Dim 8", "Dim 9", "Dim 10", "Dim 11", "Dim 12", "Dim 13");


	RegisterOperation("array_set_int", ArraySetInt, Both, None, 3, 16,
		"Writes <1> to the array with <0> at the specified index.",
		"arrayID", "value", "Dim 0", "Dim 1", "Dim 2", "Dim 3", "Dim 4", "Dim 5", "Dim 6", "Dim 7", "Dim 8", "Dim 9", "Dim 10", "Dim 11", "Dim 12", "Dim 13");

	RegisterOperation("array_set_str", ArraySetStr, Both, None, 3, 16,
		"Writes <1> to the array with <0> at the specified index.",
		"arrayID", "string", "Dim 0", "Dim 1", "Dim 2", "Dim 3", "Dim 4", "Dim 5", "Dim 6", "Dim 7", "Dim 8", "Dim 9", "Dim 10", "Dim 11", "Dim 12", "Dim 13");

	RegisterOperation("array_set_pos", ArraySetPos, Both, None, 3, 16,
		"Writes <1> to the array with <0> at the specified index.",
		"arrayID", "position_register", "Dim 0", "Dim 1", "Dim 2", "Dim 3", "Dim 4", "Dim 5", "Dim 6", "Dim 7", "Dim 8", "Dim 9", "Dim 10", "Dim 11", "Dim 12", "Dim 13");


	RegisterOperation("array_get_int", ArrayGetInt, Both, Lhs, 3, 16,
		"Gets a value from the array with <1> at the specified index and writes it to <0>.",
		"destination", "arrayID", "Dim 0", "Dim 1", "Dim 2", "Dim 3", "Dim 4", "Dim 5", "Dim 6", "Dim 7", "Dim 8", "Dim 9", "Dim 10", "Dim 11", "Dim 12", "Dim 13");

	RegisterOperation("array_get_str", ArrayGetStr, Both, None, 3, 16,
		"Gets a string from the array with <1> at the specified index and writes it to <0>.",
		"string_register", "arrayID", "Dim 0", "Dim 1", "Dim 2", "Dim 3", "Dim 4", "Dim 5", "Dim 6", "Dim 7", "Dim 8", "Dim 9", "Dim 10", "Dim 11", "Dim 12", "Dim 13");

	RegisterOperation("array_get_pos", ArrayGetPos, Both, None, 3, 16,
		"Gets a position from the array with <1> at the specified index and writes it to <0>.",
		"position_register", "arrayID", "Dim 0", "Dim 1", "Dim 2", "Dim 3", "Dim 4", "Dim 5", "Dim 6", "Dim 7", "Dim 8", "Dim 9", "Dim 10", "Dim 11", "Dim 12", "Dim 13");


	RegisterOperation("array_resize", ArrayResize, Both, None, 2, 2,
		"Changes the size of the first dimension (Dim 0) of the array with <0> to <1>.",
		"arrayID", "size");

	RegisterOperation("array_get_dim_size", ArrayGetDimSize, Both, Lhs, 2, 2,
		"Gets the size of the dimension specified by <1> of the array with <0>.",
		"arrayID", "index");

	RegisterOperation("array_get_dim_count", ArrayGetDimCount, Both, Lhs, 1, 1,
		"Gets the the amount of dimensions of the array with <0>.",
		"arrayID");
}

void WSEArrayOperationsContext::OnUnload()
{
	WSE->Objects.FreeType(m_type);
}