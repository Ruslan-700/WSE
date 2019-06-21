#include "WSEArrayOperationsContext.h"

#include "WSE.h"
#include "warband.h"
#include "WSEDynMultiArray.h"
#include <string>
#include <algorithm>

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

void CheckReg(WSEArrayOperationsContext *context, int reg)
{
	if (reg < 0 || reg >= 128)
		context->ScriptError("invalid register [%i]", reg);
}



bool loadIntIndexFromFile(int &val, const std::string &file, int index)
{
	std::ifstream stream(file, std::ios::binary);

	if (!stream.is_open())
		return false;

	stream.seekg(index * sizeof(int));
	stream.read((char*)&val, sizeof(int));
	stream.close();

	return true;
}

bool isArrayFile(const std::string &file)
{
	int magic;

	if (!loadIntIndexFromFile(magic, file, 0))
		return false;

	if (magic != WSE_ARRAY_MAGIC)
		return false;

	return true;
}

int getVersionFromFile(const std::string &file)
{
	int version;

	if (!loadIntIndexFromFile(version, file, 1))
		return -1;

	return version;
}

type_id getDataTypeIdFromFile(const std::string &file)
{
	int id;

	if (!loadIntIndexFromFile(id, file, 2))
		return type_id::invalid;

	return (type_id)id;
}


void WriteIntToBuff(WSESimpleCharBuff &buffer, const void *valPtr)
{
	buffer.write((char*)valPtr, sizeof(int));
}

void ReadIntFromMemory(char *ptr, int size, int &val)
{
	val = *((int*)ptr);
}


void WriteStrToBuff(WSESimpleCharBuff &buffer, const void *valPtr)
{
	std::string val = *((std::string *)valPtr);
	buffer.write(val.c_str(), val.length());
}

void ReadStrFromMemory(char *ptr, int size, std::string &val)
{
	val = std::string(ptr);
}


void WriteVec4ToBuff(WSESimpleCharBuff &buffer, const rgl::vector4 &vec)
{
	buffer.write((char*)&vec.x, sizeof(float));
	buffer.write((char*)&vec.y, sizeof(float));
	buffer.write((char*)&vec.z, sizeof(float));
}

void WritePosToBuff(WSESimpleCharBuff &buffer, const void *valPtr)
{
	rgl::matrix val = *((rgl::matrix *)valPtr);

	WriteVec4ToBuff(buffer, val.o);
	WriteVec4ToBuff(buffer, val.rot.f);
	WriteVec4ToBuff(buffer, val.rot.u);
}

void ReadPosFromMemory(char *ptr, int size, rgl::matrix &val)
{
	float *data = (float*)ptr;

	val.o = rgl::vector4(data[0], data[1], data[2]);
	val.rot.f = rgl::vector4(data[3], data[4], data[5]);
	val.rot.u = rgl::vector4(data[6], data[7], data[8]);

	val.orthonormalize();
}







int ArrayCreate(WSEArrayOperationsContext *context)
{
	int typeId;
	std::vector<int> dims;

	context->ExtractValue(typeId);
	context->ExtractVector(dims, -1);

	if ((dims.size() > 0) && (dims.size() <= MAX_DIMENSIONS)){
		if (typeId == type_id::num){
			WSEDynMultiArray<int> *ptr = new WSEDynMultiArray<int>(dims, (type_id)typeId);
			if (ptr->isValid())
				return WSE->Objects.AllocObject(context->m_type, (void*)ptr);
			delete ptr;
		}
		else if (typeId == type_id::str){
			WSEDynMultiArray<std::string> *ptr = new WSEDynMultiArray<std::string>(dims, (type_id)typeId);
			if (ptr->isValid())
				return WSE->Objects.AllocObject(context->m_type, (void*)ptr);
			delete ptr;
		}
		else if (typeId == type_id::pos){
			WSEDynMultiArray<rgl::matrix> *ptr = new WSEDynMultiArray<rgl::matrix>(dims, (type_id)typeId);
			if (ptr->isValid())
				return WSE->Objects.AllocObject(context->m_type, (void*)ptr);
			delete ptr;
		}
		else{
			context->ScriptError("invalid array type ID [%i]", typeId);
		}
	}

	context->ScriptError("invalid array dimensions: " + IntVecToStr(dims));
	return 0; //won't happen but ide will shut up
}

void ArrayFree(WSEArrayOperationsContext *context)
{
	int id;

	context->ExtractValue(id);
	WSEBasicDynMultiArray *ptr = (WSEBasicDynMultiArray *)context->GetArray(id);

	delete ptr;

	WSE->Objects.FreeObject(context->m_type, id);
}

int ArrayCopy(WSEArrayOperationsContext *context)
{
	int srcId, typeId;

	context->ExtractValue(srcId);
	WSEBasicDynMultiArray *srcPtr = (WSEBasicDynMultiArray*)context->GetArray(srcId);

	typeId = srcPtr->getTypeID();

	if (typeId == type_id::num){
		WSEDynMultiArray<int> *src = (WSEDynMultiArray<int> *) srcPtr;

		WSEDynMultiArray<int> *ptr = new WSEDynMultiArray<int>(src);

		if (ptr->isValid())
			return WSE->Objects.AllocObject(context->m_type, (void*)ptr);
		delete ptr;
	}
	else if (typeId == type_id::str){
		WSEDynMultiArray<std::string> *src = (WSEDynMultiArray<std::string> *) srcPtr;

		WSEDynMultiArray<std::string> *ptr = new WSEDynMultiArray<std::string>(src);

		if (ptr->isValid())
			return WSE->Objects.AllocObject(context->m_type, (void*)ptr);
		delete ptr;
	}
	else if (typeId == type_id::pos){
		WSEDynMultiArray<rgl::matrix> *src = (WSEDynMultiArray<rgl::matrix> *) srcPtr;

		WSEDynMultiArray<rgl::matrix> *ptr = new WSEDynMultiArray<rgl::matrix>(src);

		if (ptr->isValid())
			return WSE->Objects.AllocObject(context->m_type, (void*)ptr);
		delete ptr;
	}

	context->ScriptError("failed to copy array [%i]", srcId);
	return 0; //won't happen
}

void ArraySaveToFile(WSEArrayOperationsContext *context)
{
	int id, typeId;
	std::string path;

	context->ExtractValue(id);
	context->ExtractPath(path);

	WSEBasicDynMultiArray *ptr = (WSEBasicDynMultiArray *)context->GetArray(id);

	if (!ptr->isValid())
		context->ScriptError("array is not valid");

	std::string file = context->CreateFile(path, "wsearray");

	typeId = ptr->getTypeID();

	bool succ;
	if (typeId == type_id::num){
		succ = ptr->saveToFile(file, WriteIntToBuff);
	}
	else if (typeId == type_id::str){
		succ = ptr->saveToFile(file, WriteStrToBuff);
	}
	else if (typeId == type_id::pos){
		succ = ptr->saveToFile(file, WritePosToBuff);
	}

	if (!succ)
		context->ScriptError("failed to save array");
}

int ArrayLoadFromFile(WSEArrayOperationsContext *context)
{
	std::string path;
	context->ExtractPath(path);

	std::string file = context->CreateFile(path, "wsearray");

	if (!isArrayFile(file))
		context->ScriptError("could not open as array file: %s", path.c_str());

	int version = getVersionFromFile(file);
	if (version > WSE_ARRAY_VERSION)
		context->ScriptError("array file version is newer than the current version");

	type_id typeId = getDataTypeIdFromFile(file);

	if (typeId == type_id::num){
		WSEDynMultiArray<int> *ptr = new WSEDynMultiArray<int>(file, ReadIntFromMemory, typeId);
		if (ptr->isValid())
			return WSE->Objects.AllocObject(context->m_type, (void*)ptr);
		delete ptr;
	}
	else if (typeId == type_id::str){
		WSEDynMultiArray<std::string> *ptr = new WSEDynMultiArray<std::string>(file, ReadStrFromMemory, typeId);
		if (ptr->isValid())
			return WSE->Objects.AllocObject(context->m_type, (void*)ptr);
		delete ptr;
	}
	else if (typeId == type_id::pos){
		WSEDynMultiArray<rgl::matrix> *ptr = new WSEDynMultiArray<rgl::matrix>(file, ReadPosFromMemory, typeId);
		if (ptr->isValid())
			return WSE->Objects.AllocObject(context->m_type, (void*)ptr);
		delete ptr;
	}

	context->ScriptError("failed to create array from file");
	return 0; //won't happen
}

void ArrayDeleteFile(WSEDictionaryOperationsContext *context)
{
	std::string path;

	context->ExtractPath(path);

	DeleteFile(context->CreateFile(path, "wsearray").c_str());
}

void ArraySetVal(WSEArrayOperationsContext *context)
{
	int id;
	std::vector <int> indices;

	context->ExtractValue(id);
	void *ptr = context->GetArray(id);

	int typeID = ((WSEBasicDynMultiArray *)ptr)->getTypeID();

	if (typeID == type_id::num){
		WSEDynMultiArray<int> *array = (WSEDynMultiArray<int> *) ptr;

		int value;
		context->ExtractValue(value);
		context->ExtractVector(indices, -1);

		if (!array->setIndex(indices, value))
			context->ScriptError("failed to set index: " + IntVecToStr(indices));
	}
	else if (typeID == type_id::str){
		WSEDynMultiArray<std::string> *array = (WSEDynMultiArray <std::string> *) ptr;

		std::string value;
		context->ExtractString(value);
		context->ExtractVector(indices, -1);

		if (!array->setIndex(indices, value))
			context->ScriptError("failed to set index: " + IntVecToStr(indices));
	}
	else if (typeID == type_id::pos){
		WSEDynMultiArray<rgl::matrix> *array = (WSEDynMultiArray <rgl::matrix> *) ptr;

		int preg;
		rgl::matrix value;

		context->ExtractRegister(preg);
		value = warband->basic_game.position_registers[preg];
		context->ExtractVector(indices, -1);

		if (!array->setIndex(indices, value))
			context->ScriptError("failed to set index: " + IntVecToStr(indices));
	}
}

void ArrayGetVal(WSEArrayOperationsContext *context)
{
	int id, dreg;
	std::vector <int> indices;

	context->ExtractValue(dreg);
	context->ExtractValue(id);
	context->ExtractVector(indices, -1);
	void *ptr = context->GetArray(id);

	int typeID = ((WSEBasicDynMultiArray *)ptr)->getTypeID();

	if (typeID == type_id::num){
		WSEDynMultiArray<int> *array = (WSEDynMultiArray<int> *) ptr;

		const int *value = array->getIndex(indices);
		if (value == NULL)
			context->ScriptError("failed to get index: " + IntVecToStr(indices));

		context->SetReturnValue(*value);
	}
	else if (typeID == type_id::str){
		CheckReg(context, dreg);

		WSEDynMultiArray<std::string> *array = (WSEDynMultiArray <std::string> *) ptr;

		const std::string *value = array->getIndex(indices);
		if (value == NULL)
			context->ScriptError("failed to get index: " + IntVecToStr(indices));

		warband->basic_game.string_registers[dreg] = *value;
	}
	else if (typeID == type_id::pos){
		CheckReg(context, dreg);

		WSEDynMultiArray<rgl::matrix> *array = (WSEDynMultiArray <rgl::matrix> *) ptr;

		const rgl::matrix *value = array->getIndex(indices);
		if (value == NULL)
			context->ScriptError("failed to get index: " + IntVecToStr(indices));

		warband->basic_game.position_registers[dreg] = *value;
	}
}

void ArrayPush(WSEArrayOperationsContext *context)
{
	int destId;
	context->ExtractValue(destId);

	WSEBasicDynMultiArray *baseDestArray = (WSEBasicDynMultiArray *)context->GetArray(destId);

	int destTypeID = baseDestArray->getTypeID();

	if (baseDestArray->getDimCount() > 1){
		int srcId;
		context->ExtractValue(srcId);

		WSEBasicDynMultiArray *baseSrcArray = (WSEBasicDynMultiArray *)context->GetArray(srcId);


		if (destTypeID != baseSrcArray->getTypeID()){
			context->ScriptError("incompatible types");
		}

		if (destTypeID == type_id::num){
			WSEDynMultiArray<int> *destArray = (WSEDynMultiArray<int> *) baseDestArray;
			WSEDynMultiArray<int> *srcArray = (WSEDynMultiArray<int> *) baseSrcArray;

			if (!destArray->push_back(*srcArray)){
				context->ScriptError("failed to push array [%i] on array [%i]", srcId, destId);
			}
		}
		else if (destTypeID == type_id::str){
			WSEDynMultiArray<std::string> *destArray = (WSEDynMultiArray<std::string> *) baseDestArray;
			WSEDynMultiArray<std::string> *srcArray = (WSEDynMultiArray<std::string> *) baseSrcArray;

			if (!destArray->push_back(*srcArray)){
				context->ScriptError("failed to push array [%i] on array [%i]", srcId, destId);
			}
		}
		else if (destTypeID == type_id::pos){
			WSEDynMultiArray<rgl::matrix> *destArray = (WSEDynMultiArray<rgl::matrix> *) baseDestArray;
			WSEDynMultiArray<rgl::matrix> *srcArray = (WSEDynMultiArray<rgl::matrix> *) baseSrcArray;

			if (!destArray->push_back(*srcArray)){
				context->ScriptError("failed to push array [%i] on array [%i]", srcId, destId);
			}
		}
	}
	else if (baseDestArray->getDimCount() == 1){
		if (destTypeID == type_id::num){
			WSEDynMultiArray<int> *destArray = (WSEDynMultiArray<int> *) baseDestArray;

			int val;
			context->ExtractValue(val);

			if (!destArray->push_back(val)){
				context->ScriptError("failed to push int on array [%i]", destId);
			}
		}
		else if (destTypeID == type_id::str){
			WSEDynMultiArray<std::string> *destArray = (WSEDynMultiArray<std::string> *) baseDestArray;

			std::string val;
			context->ExtractString(val);

			if (!destArray->push_back(val)){
				context->ScriptError("failed to push string on array [%i]", destId);
			}
		}
		else if (destTypeID == type_id::pos){
			WSEDynMultiArray<rgl::matrix> *destArray = (WSEDynMultiArray<rgl::matrix> *) baseDestArray;

			int pReg;
			context->ExtractRegister(pReg);

			if (!destArray->push_back(warband->basic_game.position_registers[pReg])){
				context->ScriptError("failed to push position on array [%i]", destId);
			}
		}
	}
}

void ArrayPop(WSEArrayOperationsContext *context)
{
	int dReg, id;
	context->ExtractValue(dReg);
	context->ExtractValue(id);

	WSEBasicDynMultiArray *baseArray = (WSEBasicDynMultiArray *)context->GetArray(id);

	int typeId = baseArray->getTypeID();

	if (baseArray->isMultidim()){
		void *newArray = baseArray->pop_back_multi();

		if (newArray == NULL)
			context->ScriptError("failed to pop array from multidimensional array [%i]", id);

		context->SetReturnValue(WSE->Objects.AllocObject(context->m_type, newArray));
	}
	else{
		if (baseArray->getTypeID() == type_id::num){
			WSEDynMultiArray<int> *arr = (WSEDynMultiArray<int> *)baseArray;

			int *val = arr->pop_back_single();

			if (val == NULL)
				context->ScriptError("failed to pop int from array [%i]", id);

			context->SetReturnValue(*val);
			delete val;
		}
		else if (baseArray->getTypeID() == type_id::str){
			CheckReg(context, dReg);
			WSEDynMultiArray<std::string> *arr = (WSEDynMultiArray<std::string> *)baseArray;

			std::string *val = arr->pop_back_single();

			if (val == NULL)
				context->ScriptError("failed to pop string from array [%i]", id);

			warband->basic_game.string_registers[dReg] = *val;
			delete val;
		}
		else if (baseArray->getTypeID() == type_id::pos){
			CheckReg(context, dReg);
			WSEDynMultiArray<rgl::matrix> *arr = (WSEDynMultiArray<rgl::matrix> *)baseArray;

			rgl::matrix *val = arr->pop_back_single();

			if (val == NULL)
				context->ScriptError("failed to pop position from array [%i]", id);

			warband->basic_game.position_registers[dReg] = *val;
			delete val;
		}

	}
}

void ArraySetValAll(WSEArrayOperationsContext *context)
{
	int id;

	context->ExtractValue(id);
	void *ptr = context->GetArray(id);

	int typeID = ((WSEBasicDynMultiArray *)ptr)->getTypeID();

	if (typeID == type_id::num){
		WSEDynMultiArray<int> *array = (WSEDynMultiArray<int> *) ptr;

		int value;
		context->ExtractValue(value);

		if (!array->setAllValues(value))
			context->ScriptError("failed to set all values");
	}
	else if (typeID == type_id::str){
		WSEDynMultiArray<std::string> *array = (WSEDynMultiArray <std::string> *) ptr;

		std::string value;
		context->ExtractString(value);

		if (!array->setAllValues(value))
			context->ScriptError("failed to set all values");
	}
	else if (typeID == type_id::pos){
		WSEDynMultiArray<rgl::matrix> *array = (WSEDynMultiArray <rgl::matrix> *) ptr;

		int preg;
		rgl::matrix value;

		context->ExtractRegister(preg);
		value = warband->basic_game.position_registers[preg];

		if (!array->setAllValues(value))
			context->ScriptError("failed to set all values");
	}
}

void ArrayResizeDim(WSEArrayOperationsContext *context)
{
	int id, dimIndex, size;

	context->ExtractValue(id);
	context->ExtractValue(dimIndex);
	context->ExtractValue(size);

	WSEBasicDynMultiArray *array = (WSEBasicDynMultiArray *)context->GetArray(id);

	if (!array->resizeDim(dimIndex, (size_t)size)){
		context->ScriptError("failed to resize array [%i]", id);
	}
}

int ArrayGetDimSize(WSEArrayOperationsContext *context)
{
	int id, index;
	context->ExtractValue(id);
	context->ExtractValue(index);

	WSEBasicDynMultiArray *array = (WSEBasicDynMultiArray *)context->GetArray(id);

	return array->getDimSize(index);
}

int ArrayGetDimCount(WSEArrayOperationsContext *context)
{
	int id;
	context->ExtractValue(id);

	WSEBasicDynMultiArray *array = (WSEBasicDynMultiArray *)context->GetArray(id);

	return array->getDimCount();
}

int ArrayGetTypeId(WSEArrayOperationsContext *context)
{
	int id;
	context->ExtractValue(id);

	WSEBasicDynMultiArray *array = (WSEBasicDynMultiArray *)context->GetArray(id);

	return array->getTypeID();
}

void ArraySort(WSEArrayOperationsContext *context)
{
	int id, sortMode;
	std::vector<int> indices;

	context->ExtractValue(id);
	context->ExtractValue(sortMode);
	context->ExtractVector(indices, -1);

	WSEBasicDynMultiArray *ptr = (WSEBasicDynMultiArray *)context->GetArray(id);

	type_id typeID = ptr->getTypeID();
	int end = ptr->getDimSize(0) - 1;

	if (typeID == type_id::num){
		if(!context->sortOptions.setSortMode(sortMode, 1, false))
			context->ScriptError("invalid sort mode");

		WSEDynMultiArray<int> *array = (WSEDynMultiArray<int> *) ptr;

		if (!array->sort(indices, cmpInt, context->sortOptions, 0, end))
			context->ScriptError("failed to sort array");
	}
	else if (typeID == type_id::str){
		if (!context->sortOptions.setSortMode(sortMode, 3, false))
			context->ScriptError("invalid sort mode");

		WSEDynMultiArray<std::string> *array = (WSEDynMultiArray <std::string> *) ptr;

		if (!array->sort(indices, cmpStr, context->sortOptions, 0, end))
			context->ScriptError("failed to sort array");
	}
	else {
		context->ScriptError("not supported for this type of array");
	}
}

void ArraySortCustom(WSEArrayOperationsContext *context)
{
	int id, cmp_script_no;
	std::vector<int> indices;

	context->ExtractValue(id);
	context->ExtractValue(cmp_script_no);
	context->ExtractVector(indices, -1);

	if (!context->sortOptions.setSortMode(cmp_script_no, 0, true))
		context->ScriptError("invalid script no");

	WSEBasicDynMultiArray *ptr = (WSEBasicDynMultiArray *)context->GetArray(id);

	type_id typeID = ptr->getTypeID();
	int end = ptr->getDimSize(0) - 1;

	if (typeID == type_id::num){
		WSEDynMultiArray<int> *array = (WSEDynMultiArray<int> *) ptr;

		if (!array->sort(indices, cmpInt, context->sortOptions, 0, end))
			context->ScriptError("failed to sort array");
	}
	else if (typeID == type_id::str){
		WSEDynMultiArray<std::string> *array = (WSEDynMultiArray <std::string> *) ptr;

		if (!array->sort(indices, cmpStr, context->sortOptions, 0, end))
			context->ScriptError("failed to sort array");
	}
	else {
		WSEDynMultiArray<rgl::matrix> *array = (WSEDynMultiArray <rgl::matrix> *) ptr;

		if (!array->sort(indices, cmpPos, context->sortOptions, 0, end))
			context->ScriptError("failed to sort array");
	}
}

bool ArrayCompare(WSEArrayOperationsContext *context, cmp_operation op)
{
	int id, cmpVal;
	std::vector<int> indices;

	context->ExtractValue(id);

	WSEBasicDynMultiArray *ptr = (WSEBasicDynMultiArray *)context->GetArray(id);

	type_id typeID = ptr->getTypeID();

	if (typeID == type_id::num){
		WSEDynMultiArray<int> *array = (WSEDynMultiArray<int> *) ptr;

		context->ExtractValue(cmpVal);
		context->ExtractVector(indices, -1);

		const int *value = array->getIndex(indices);
		if (value == NULL)
			context->ScriptError("failed to get index: " + IntVecToStr(indices));

		if (op == cmp_operation::eq){
			return *value == cmpVal;
		}
		else if (op == cmp_operation::neq){
			return *value != cmpVal;
		}
		else if (op == cmp_operation::gt){
			return *value > cmpVal;
		}
		else if (op == cmp_operation::ge){
			return *value >= cmpVal;
		}
		else if (op == cmp_operation::lt){
			return *value < cmpVal;
		}
		else if (op == cmp_operation::le){
			return *value <= cmpVal;
		}
	}
	else if (typeID == type_id::str){
		WSEDynMultiArray<std::string> *array = (WSEDynMultiArray <std::string> *) ptr;

		std::string cmpStr;
		context->ExtractString(cmpStr);
		context->ExtractVector(indices, -1);

		const std::string *value = array->getIndex(indices);
		if (value == NULL)
			context->ScriptError("failed to get index: " + IntVecToStr(indices));
		
		if (op == cmp_operation::eq){
			return *value == cmpStr;
		}
		else if (op == cmp_operation::neq){
			return *value != cmpStr;
		}
		else if (op == cmp_operation::gt){
			return *value > cmpStr;
		}
		else if (op == cmp_operation::ge){
			return *value >= cmpStr;
		}
		else if (op == cmp_operation::lt){
			return *value < cmpStr;
		}
		else if (op == cmp_operation::le){
			return *value <= cmpStr;
		}
	}
	else {
		WSEDynMultiArray<rgl::matrix> *array = (WSEDynMultiArray <rgl::matrix> *) ptr;

		context->ExtractRegister(cmpVal);
		context->ExtractVector(indices, -1);

		const rgl::matrix *value = array->getIndex(indices);
		if (value == NULL)
			context->ScriptError("failed to get index: " + IntVecToStr(indices));

		rgl::matrix pos = *value;
		rgl::matrix pos1 = warband->basic_game.position_registers[cmpVal];

		pos.orthonormalize();
		pos1.orthonormalize();


		if (op == cmp_operation::eq){
			return pos == pos1;
		}
		else if (op == cmp_operation::neq){
			return pos != pos1;
		}
		else{
			context->ScriptError("invalid operation for pos array");
		}
	}

	return false;
}

bool ArrayEq(WSEArrayOperationsContext *context)
{
	return ArrayCompare(context, cmp_operation::eq);
}

bool ArrayNeq(WSEArrayOperationsContext *context)
{
	return ArrayCompare(context, cmp_operation::neq);
}

bool ArrayGt(WSEArrayOperationsContext *context)
{
	return ArrayCompare(context, cmp_operation::gt);
}

bool ArrayGe(WSEArrayOperationsContext *context)
{
	return ArrayCompare(context, cmp_operation::ge);
}

bool ArrayLt(WSEArrayOperationsContext *context)
{
	return ArrayCompare(context, cmp_operation::lt);
}

bool ArrayLe(WSEArrayOperationsContext *context)
{
	return ArrayCompare(context, cmp_operation::le);
}

WSEArrayOperationsContext::WSEArrayOperationsContext() : WSEOperationContext("array", 5000, 5099)
{
}

void *WSEArrayOperationsContext::GetArray(int id)
{
	void *array = WSE->Objects.FetchObject(m_type, id);

	if (array){
		if (((WSEBasicDynMultiArray *)array)->isValid()){
			return array;
		}
	}

	ScriptError("operand [%i] is not a valid array", id);
	return NULL;
}


void WSEArrayOperationsContext::OnLoad()
{
	m_type = WSE->Objects.AllocType("array");

	RegisterOperation("array_create", ArrayCreate, Both, Lhs, 3, 16,
		"Creates an array object of <1> (0: Integer, 1: String, 2: Position) and stores its ID into <0>. You can specify up to 14 dimensions, from <2> to <15>. The array will be initialized by default with 0 / empty string / 0-position.",
		"destination", "type_id", "Dim 0", "Dim 1", "Dim 2", "Dim 3", "Dim 4", "Dim 5", "Dim 6", "Dim 7", "Dim 8", "Dim 9", "Dim 10", "Dim 11", "Dim 12", "Dim 13");

	RegisterOperation("array_free", ArrayFree, Both, None, 1, 1,
		"Frees array with <0>.",
		"arrayID");

	RegisterOperation("array_copy", ArrayCopy, Both, Lhs, 2, 2,
		"Copys array with <1> and stores the new array id into <0>.",
		"destination", "source arrayID");

	RegisterOperation("array_save_file", ArraySaveToFile, Both, None, 2, 2,
		"Saves <0> into a file. For security reasons, <1> is just a name, not a full path, and will be stored into a WSE managed directory.",
		"arrayID", "file");

	RegisterOperation("array_load_file", ArrayLoadFromFile, Both, Lhs, 2, 2,
		"Loads <1> as an array and stores the newly created array's ID into <0>.",
		"destination", "file");

	RegisterOperation("array_delete_file", ArrayDeleteFile, Both, None, 1, 1,
		"Deletes array <0>.",
		"file");

	RegisterOperation("array_set_val", ArraySetVal, Both, None, 3, 16,
		"Writes <1> to the array with <0> at the specified index. <1> can be an integer, a position register or a string register and must match the type of the array.",
		"arrayID", "value", "Index 0", "Index 1", "Index 2", "Index 3", "Index 4", "Index 5", "Index 6", "Index 7", "Index 8", "Index 9", "Index 10", "Index 11", "Index 12", "Index 13");

	RegisterOperation("array_set_val_all", ArraySetValAll, Both, None, 2, 2,
		"Writes <1> to all indices of the array with <0>. <1> can be an integer, a position register or a string register and must match the type of the array.",
		"arrayID", "value");

	RegisterOperation("array_get_val", ArrayGetVal, Both, FakeLhs, 3, 16,
		"Gets a value from the array with <1> at the specified index and writes it to <0>. <0> can be a variable, a position register or a string register and must match the type of the array.",
		"destination", "arrayID", "Index 0", "Index 1", "Index 2", "Index 3", "Index 4", "Index 5", "Index 6", "Index 7", "Index 8", "Index 9", "Index 10", "Index 11", "Index 12", "Index 13");

	RegisterOperation("array_push", ArrayPush, Both, None, 2, 2,
		"Pushes <1> on the array with <0>. If <0> is a 1D array, <1> can be an int, string, or position register and must match the type of <0>. If <0> is multidimensional, <1> must be the id of an array with matching type, src dimension count = dest dimension count - 1, and dimension sizes src_dim_0_size = dest_dim_1_size ... src_dim_n_size = dest_dim_n+1_size.",
		"destination arrayID", "source");

	RegisterOperation("array_pop", ArrayPop, Both, FakeLhs, 2, 2,
		"Pops the last value  from the array with <1>. If <1> is a 1D array, <0> must be a variable, string, or position register and must match the type of <1>. If <1> is multidimensional, a new array with dimension count = src dimension count - 1, dimensions dim_0 = src_dim_1 ... dim_n = src_dim_n+1 will be created and its ID will be stored in <0>",
		"destination", "arrayID");

	RegisterOperation("array_resize_dim", ArrayResizeDim, Both, None, 3, 3,
		"Changes the size of the dimension with <1> of the array with <0> to <2>.",
		"arrayID", "dimIndex", "size");

	RegisterOperation("array_get_dim_size", ArrayGetDimSize, Both, Lhs, 3, 3,
		"Gets the size of the dimension with <2> of the array with <1> and stores it into <0>.",
		"destination", "arrayID", "dimIndex");

	RegisterOperation("array_get_dim_count", ArrayGetDimCount, Both, Lhs, 2, 2,
		"Gets the the amount of dimensions of the array with <1> and stores it into <0>.",
		"destination", "arrayID");

	RegisterOperation("array_get_type_id", ArrayGetTypeId, Both, Lhs, 2, 2,
		"Gets the the type id of the array with <1> and stores it into <0>.", 
		"destination", "arrayID");

	RegisterOperation("array_sort", ArraySort, Both, None, 2, 15,
		"Sorts the array with <0> using a stable natural-mergesort algorithm. <1> can be: [sort_m_int_asc or sort_m_int_desc] for int, [sort_m_str_cs_asc, sort_m_str_cs_desc, sort_m_str_ci_asc, sort_m_str_ci_desc] for str (asc=ascending, desc=descending, cs=case sensitive, ci=case insensitive, strings are compared alphabetically, upper before lower case). If the array is multidimensional, only the first dimension will be sorted and you must specify (dim_count - 1) fixed indices that will be used for access.",
		"arrayID", "sortMode", "Index 1", "Index 2", "Index 3", "Index 4", "Index 5", "Index 6", "Index 7", "Index 8", "Index 9", "Index 10", "Index 11", "Index 12", "Index 13");

	RegisterOperation("array_sort_custom", ArraySortCustom, Both, None, 2, 15,
		"Sorts the array with <0> using a stable natural-mergesort algorithm. <1> must compare its two input values (reg0 and reg1 / s0 and s1 / pos0 and pos1) and use (return_values, x) where x is nonzero if the first value goes before or is equal to the second, and zero otherwise. If the array is multidimensional, only the first dimension will be sorted and you must specify (dim_count - 1) fixed indices that will be used for access. The sorting won't be successful if the compare script does not work properly. The algorithm will abort at some point and not go into an infinite loop, it may however take extremely long to finish on big arrays.",
		"arrayID", "cmpScript", "Index 1", "Index 2", "Index 3", "Index 4", "Index 5", "Index 6", "Index 7", "Index 8", "Index 9", "Index 10", "Index 11", "Index 12", "Index 13");

	RegisterOperation("array_eq", ArrayEq, Both, Cf, 3, 16,
		"Fails if the specified value in the array with <0> is not equal to <1>. Works for int, str and pos.",
		"arrayID", "value_1", "Index 0", "Index 1", "Index 2", "Index 3", "Index 4", "Index 5", "Index 6", "Index 7", "Index 8", "Index 9", "Index 10", "Index 11", "Index 12", "Index 13");

	RegisterOperation("array_neq", ArrayNeq, Both, Cf, 3, 16,
		"Fails if the specified value in the array with <0> is equal to <1>. Works for int, str and pos.",
		"arrayID", "value_1", "Index 0", "Index 1", "Index 2", "Index 3", "Index 4", "Index 5", "Index 6", "Index 7", "Index 8", "Index 9", "Index 10", "Index 11", "Index 12", "Index 13");

	RegisterOperation("array_gt", ArrayGt, Both, Cf, 3, 16,
		"Fails if the specified value in the array with <0> is not greater than <1>. Works for int and str. Strings are compared alphabetically, upper before lower case.",
		"arrayID", "value_1", "Index 0", "Index 1", "Index 2", "Index 3", "Index 4", "Index 5", "Index 6", "Index 7", "Index 8", "Index 9", "Index 10", "Index 11", "Index 12", "Index 13");

	RegisterOperation("array_ge", ArrayGe, Both, Cf, 3, 16,
		"Fails if the specified value in the array with <0> is not greater or equal to <1>. Works for int and str. Strings are compared alphabetically, upper before lower case.",
		"arrayID", "value_1", "Index 0", "Index 1", "Index 2", "Index 3", "Index 4", "Index 5", "Index 6", "Index 7", "Index 8", "Index 9", "Index 10", "Index 11", "Index 12", "Index 13");

	RegisterOperation("array_lt", ArrayLt, Both, Cf, 3, 16,
		"Fails if the specified value in the array with <0> is not lower than <1>. Works for int and str. Strings are compared alphabetically, upper before lower case.",
		"arrayID", "value_1", "Index 0", "Index 1", "Index 2", "Index 3", "Index 4", "Index 5", "Index 6", "Index 7", "Index 8", "Index 9", "Index 10", "Index 11", "Index 12", "Index 13");

	RegisterOperation("array_le", ArrayLe, Both, Cf, 3, 16,
		"Fails if the specified value in the array with <0> is not lower or equal to <1>. Works for int and str. Strings are compared alphabetically, upper before lower case.",
		"arrayID", "value_1", "Index 0", "Index 1", "Index 2", "Index 3", "Index 4", "Index 5", "Index 6", "Index 7", "Index 8", "Index 9", "Index 10", "Index 11", "Index 12", "Index 13");
}

void WSEArrayOperationsContext::OnUnload()
{
	WSE->Objects.FreeType(m_type);
}


bool WSEArraySortOptions::setSortMode(int mode, int maxMode, bool isCustomScript)
{
	if (isCustomScript){
		if (mode < 0 || mode >= warband->script_manager.num_scripts)
			return false;

		sortCmpScriptNo = mode;
	}
	else{
		sortCmpScriptNo = -1;

		if (mode < 0 || mode > maxMode)
			return false;

		sortDescending = (mode & SORT_F_DESCENDING) == SORT_F_DESCENDING;
		sortCaseInsensitive = (mode & SORT_F_CASE_INSENSITIVE) == SORT_F_CASE_INSENSITIVE;
	}

	return true;
}

bool cmpInt(const int &a, const int &b, size_t index1, size_t index2, const WSEArraySortOptions &opt)
{
	if (opt.sortCmpScriptNo >= 0){
		warband->basic_game.registers[0] = a;
		warband->basic_game.registers[1] = b;
		return customCmp(index1, index2, opt);
	}

	if (opt.sortDescending)
		return b <= a;

	return a <= b;
}

bool cmpStr(const std::string &a, const std::string &b, size_t index1, size_t index2, const WSEArraySortOptions &opt)
{
	if (opt.sortCmpScriptNo >= 0){
		warband->basic_game.string_registers[0] = a;
		warband->basic_game.string_registers[1] = b;
		return customCmp(index1, index2, opt);
	}

	if (opt.sortCaseInsensitive){
		std::string a1(a.size(), 0);
		std::string b1(b.size(), 0);

		std::transform(a.begin(), a.end(), a1.begin(), ::tolower);
		std::transform(b.begin(), b.end(), b1.begin(), ::tolower);

		if (opt.sortDescending)
			return b1 <= a1;

		return a1 <= b1;
	}

	if (opt.sortDescending)
		return b <= a;

	return a <= b;
}

bool cmpPos(const rgl::matrix &a, const rgl::matrix &b, size_t index1, size_t index2, const WSEArraySortOptions &opt)
{
	warband->basic_game.position_registers[0] = a;
	warband->basic_game.position_registers[1] = b;
	return customCmp(index1, index2, opt);
}

bool customCmp(size_t index1, size_t index2, const WSEArraySortOptions &opt)
{
	//WSE->Game.ExecuteScript(opt.sortCmpScriptNo, 2, arg1, arg2);
	__int64 params[16] = {
		index1,
		index2,
	};

	warband->script_manager.scripts[opt.sortCmpScriptNo].execute(2, params);

	return (WSE->CoreOperations.m_num_return_values == 1) && (WSE->CoreOperations.m_return_values[0] != 0);
}