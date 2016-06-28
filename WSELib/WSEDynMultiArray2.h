#pragma once

#include <vector>
#include <fstream>
#include <functional>
#include "WSESimpleCharBuff.h"

enum type_id{
	invalid = -1,
	num = 0,
	str = 1,
	pos = 2
};

#define WSE_ARRAY_MAGIC 0x57534541
#define WSE_ARRAY_VERSION 1

class WSEBasicDynMultiArray
{
public:
	typedef void(*write_val_f)(WSESimpleCharBuff&, const void*);
	
	WSEBasicDynMultiArray()
	{
	}

	int getDimSize(int index) const
	{
		if (index < 0 || index >= (int)dimSizes.size()){
			return -1;
		}

		return dimSizes[index];
	}

	int getDimCount() const
	{
		return dimSizes.size();
	}

	bool isValid() const
	{
		return isDimSizesValid();
	}

	bool isMultidim() const
	{
		return isPtrDim(0);
	}

	type_id getTypeID() const
	{
		return dataTypeID;
	}

	virtual bool saveToFile(const std::string &file, write_val_f write_cb) const = 0;
	virtual bool resizeDim(int dimIndex, size_t size) = 0;
	virtual void *pop_back_multi() = 0;

protected:
	void *data;
	std::vector<int> dimSizes;
	type_id dataTypeID;

	bool isDimSizesValid() const
	{
		if (dimSizes.size() < 1)
			return false;

		if (dimSizes[0] < 0)
			return false;

		//first dim size can be 0! Allowing to create empty vectors for pushing vals.
		for (size_t i = 1; i < dimSizes.size(); i++){
			if (dimSizes[i] < 1)
				return false;
		}

		return true;
	}

	bool isPtrDim(int dimIndex) const
	{
		return (dimIndex + 1) < (int)dimSizes.size();
	}

	bool isIndicesValid(const std::vector<int> &indices) const
	{
		if (indices.size() != dimSizes.size())
			return false;

		for (size_t i = 0; i < dimSizes.size(); i++){
			if ((indices[i] < 0) || (indices[i] >= dimSizes[i]))
				return false;
		}

		return true;
	}
};





template <typename T>
class WSEDynMultiArray2 : public WSEBasicDynMultiArray
{
public:
	typedef void(*read_val_f)(char*, int, T&);

	WSEDynMultiArray2(const std::vector<int> &dimensions, type_id typeID) : WSEBasicDynMultiArray()
	{
		dataTypeID = typeID;
		dimSizes = *(new std::vector<int>(dimensions));

		if (isDimSizesValid()){
			data = buildSubDimension(0);
		}
		else{
			dimSizes.clear();
		}
	}

	WSEDynMultiArray2(WSEDynMultiArray2<T> *source) : WSEBasicDynMultiArray()
	{
		dataTypeID = source->dataTypeID;
		dimSizes = *(new std::vector<int>(source->dimSizes));

		if (isDimSizesValid()){
			data = source->createPrimitiveCopy();
		}
		else{
			dimSizes.clear();
		}
	}

	WSEDynMultiArray2(std::string file, read_val_f read_cb, type_id typeID) : WSEBasicDynMultiArray()
	{
		std::ifstream stream(file, std::ios::binary);
		
		if (!stream.is_open())
			return;

		int magic, version, type;
		stream.read((char*)&magic, sizeof(int));
		stream.read((char*)&version, sizeof(int));
		stream.read((char*)&type, sizeof(int));

		if (magic != WSE_ARRAY_MAGIC)
			return;

		if (version > WSE_ARRAY_VERSION)
			return;

		if (type != typeID)
			return;

		dataTypeID = typeID;

		int dimCount;
		stream.read((char*)&dimCount, sizeof(int));

		if (dimCount < 1)
			return;

		int curDimSize;
		for (int i = 0; i < dimCount; i++){
			stream.read((char*)&curDimSize, sizeof(int));
			dimSizes.push_back(curDimSize);
		}

		if (isDimSizesValid()){
			data = buildSubDimension(0);
		}
		else{
			dimSizes.clear();
		}

		loadSubDimension(stream, read_cb, 0, data);
	}

	~WSEDynMultiArray2()
	{
		if (isDimSizesValid()){
			deleteSubDimension(data, 0);
		}

		delete &dimSizes;
	}

	bool saveToFile(const std::string &file, write_val_f write_cb) const
	{
		if (!isValid())
			return false;

		std::ofstream stream(file, std::ios::binary);

		if (!stream.is_open())
			return false;

		//Header
		int magic = WSE_ARRAY_MAGIC;
		int version = WSE_ARRAY_VERSION;
		stream.write((char *)&magic, sizeof(int));
		stream.write((char *)&version, sizeof(int));

		//Metadata
		stream.write((char *)&dataTypeID, sizeof(int));

		int dimCount = (int)dimSizes.size();
		stream.write((char *)&dimCount, sizeof(int));

		for (int i = 0; i < dimCount; i++){
			int curVal = dimSizes[i];
			stream.write((char *)&curVal, sizeof(int));
		}

		//Data
		saveSubDimension(stream, write_cb, 0, data);
		stream.close();
		return true;
	}

	const T *getIndex(const std::vector<int> &indices) const
	{
		return getIndexPtr(indices);
	}

	bool setIndex(const std::vector<int> &indices, T value)
	{
		T *ptr = getIndexPtr(indices);

		if (ptr != NULL){
			*ptr = value;
			return true;
		}
		else{
			return false;
		}
	}

	bool setAllValues(T value)
	{
		if (!isValid())
			return false;

		setSubDimensionVals(value, 0, data);
		return true;
	}

	bool resizeDim(int dimIndex, size_t size)
	{
		if (dimIndex < 0 || dimIndex >= (int)dimSizes.size()){
			return false;
		}

		if (size < 1){
			return false;
		}

		rebuildSubDimension(data, 0, dimIndex, size);
		dimSizes[dimIndex] = size;

		return true;
	}
	
	bool push_back(WSEDynMultiArray2<T> &arr)
	{
		if (!isPtrDim(0)){
			return false;
		}

		if (arr.getDimCount() != (getDimCount() - 1)){
			return false;
		}

		for (int i = 0; i < arr.getDimCount(); i++){
			if (arr.getDimSize(i) != getDimSize(i + 1)){
				return false;
			}
		}

		void *newEntry = arr.createPrimitiveCopy();

		((std::vector<void*> *)data)->push_back(newEntry);
		dimSizes[0] += 1;

		return true;
	}

	bool push_back(T val)
	{
		if (isPtrDim(0)){
			return false;
		}

		((std::vector<T> *)data)->push_back(val);
		dimSizes[0] += 1;

		return true;
	}

	void *pop_back_multi()
	{
		if (!isPtrDim(0))
			return NULL;

		std::vector<void*> *dimPtr = (std::vector<void*> *)data;

		if (dimPtr->size() < 1){
			return NULL;
		}
			
		std::vector<int> newDimSizes;
		for (size_t i = 1; i < dimSizes.size(); i++){
			newDimSizes.push_back(dimSizes[i]);
		}

		void *lastEl = dimPtr->back();
			
		WSEDynMultiArray2<T> *res = new WSEDynMultiArray2<T>(newDimSizes, dataTypeID, lastEl);

		if (!res->isValid()){
			delete res;
			return NULL;
		}

		dimPtr->pop_back();
		dimSizes[0] -= 1;
		return res;		
	}

	T *pop_back_single()
	{
		if (isPtrDim(0))
			return NULL;

		std::vector<T> *dimPtr = (std::vector<T> *)data;

		if (dimPtr->size() < 1)
			return NULL;

		T *res = new T(dimPtr->back());
		dimPtr->pop_back();
		dimSizes[0] -= 1;

		return res;
	}

	bool sort(const std::vector<int> &indices, std::function<bool(const T&, const T&)> cmp_cb, int start, int end)
	{
		if (!isValid())
			return false;

		if (indices.size() != (dimSizes.size() - 1))
			return false;

		for (size_t i = 0; i < indices.size(); i++){
			if ((indices[i] < 0) || (indices[i] >= dimSizes[i + 1]))
				return false;
		}

		if (start < 0 || start >= dimSizes[0] || start > end)
			return false;

		if (end < 0 || end >= dimSizes[0])
			return false;

		quickSort(indices, cmp_cb, start, end);
		return true;
	}

protected:
	WSEDynMultiArray2(const std::vector<int> &dimensions, type_id typeID, void *dataCopy) : WSEBasicDynMultiArray()
	{
		dataTypeID = typeID;
		dimSizes = *(new std::vector<int>(dimensions));

		if (isDimSizesValid()){
			data = dataCopy;
		}
		else{
			dimSizes.clear();
		}
	}

	void *buildSubDimension(int dimIndex) const
	{
		if (isPtrDim(dimIndex)){
			std::vector<void*> *subDim = new std::vector<void*>(dimSizes[dimIndex]);

			for (int i = 0; i < dimSizes[dimIndex]; i++){
				(*subDim)[i] = buildSubDimension(dimIndex + 1);
			}
			return (void*)subDim;
		}
		else{
			std::vector<T> *subDim = new std::vector<T>(dimSizes[dimIndex]);
			return (void*)subDim;
		}
	}

	void deleteSubDimension(void *dimPtr, int dimIndex)
	{
		if (isPtrDim(dimIndex)){
			std::vector<void*> *subDim = (std::vector<void*> *)dimPtr;

			for (int i = 0; i < dimSizes[dimIndex]; i++){
				deleteSubDimension(subDim, dimIndex + 1);
			}
			delete subDim;
		}
		else{
			std::vector<T> *subDim = (std::vector<T>*)dimPtr;
			delete subDim;
		}
	}

	void rebuildSubDimension(void *curDimRawPtr, int curDimIndex, int dimIndex, size_t size)
	{
		if (curDimIndex < dimIndex){
			std::vector<void*> *curDimPtr = (std::vector<void*> *)curDimRawPtr;

			for (size_t i = 0; i < curDimPtr->size(); i++){
				rebuildSubDimension((*curDimPtr)[i], curDimIndex + 1, dimIndex, size);
			}
		}
		else if (curDimIndex == dimIndex){
			if (isPtrDim(curDimIndex)){
				std::vector<void*> *curDimPtr = (std::vector<void*> *)curDimRawPtr;
				size_t oldSize = curDimPtr->size();

				if (oldSize < size){
					curDimPtr->resize(size);

					for (size_t i = (oldSize - 1); i < size; i++){
						(*curDimPtr)[i] = buildSubDimension(curDimIndex + 1);
					}
				}
				else if (oldSize > size){
					for (size_t i = (oldSize - 1); i >= size; i--){
						deleteSubDimension((*curDimPtr)[i], curDimIndex + 1);
					}

					curDimPtr->resize(size);
				}
			}
			else{
				std::vector<T> *curDimPtr = (std::vector<T> *)curDimRawPtr;
				curDimPtr->resize(size);
			}
		}
	}

	void copySubDimension(int curDimIndex, void *curRawSrcPtr, void *curRawDestPtr) const
	{
		if (isPtrDim(curDimIndex)){
			std::vector<void*> *curSrcPtr = (std::vector<void*> *) curRawSrcPtr;
			std::vector<void*> *curDestPtr = (std::vector<void*> *) curRawDestPtr;

			for (size_t i = 0; i < curSrcPtr->size(); i++){
				copySubDimension(curDimIndex + 1, (*curSrcPtr)[i], (*curDestPtr)[i]);
			}
		}
		else{
			std::vector<T> *curSrcPtr = (std::vector<T> *) curRawSrcPtr;
			std::vector<T> *curDestPtr = (std::vector<T> *) curRawDestPtr;

			for (size_t i = 0; i < curSrcPtr->size(); i++){
				(*curDestPtr)[i] = (*curSrcPtr)[i];
			}
		}
	}

	void saveSubDimension(std::ofstream &stream, write_val_f write_cb, int curDimIndex, void *curRawPtr) const
	{
		if (isPtrDim(curDimIndex)){
			std::vector<void *> *curPtr = (std::vector<void *> *) curRawPtr;
			
			for (size_t i = 0; i < curPtr->size(); i++){
				saveSubDimension(stream, write_cb, curDimIndex + 1, (*curPtr)[i]);
			}
		}
		else{
			std::vector<T> *curPtr = (std::vector<T> *) curRawPtr;
			WSESimpleCharBuff buffer;

			for (size_t i = 0; i < curPtr->size(); i++){
				write_cb(buffer, (void*) &(*curPtr)[i]);

				int dataSize = buffer.size();
				stream.write((char *)&dataSize, sizeof(int));
				
				if (dataSize > 0)
					stream.write(buffer.getDataPtr(), dataSize);

				buffer.clear();
			}
		}
	}

	void loadSubDimension(std::ifstream &stream, read_val_f read_cb, int curDimIndex, void *curRawPtr) const
	{
		if (isPtrDim(curDimIndex)){
			std::vector<void *> *curPtr = (std::vector<void *> *) curRawPtr;

			for (size_t i = 0; i < curPtr->size(); i++){
				loadSubDimension(stream, read_cb, curDimIndex + 1, (*curPtr)[i]);
			}
		}
		else{
			std::vector<T> *curPtr = (std::vector<T> *) curRawPtr;

			for (size_t i = 0; i < curPtr->size(); i++){
				int dataSize;
				stream.read((char*)&dataSize, sizeof(int));

				if (dataSize > 0){
					char *dataPtr = new char[dataSize];
					stream.read(dataPtr, dataSize);

					read_cb(dataPtr, dataSize, (*curPtr)[i]);

					delete[] dataPtr;
				}
			}
		}
	}

	void setSubDimensionVals(T value, int curDimIndex, void *curRawPtr) const
	{
		if (isPtrDim(curDimIndex)){
			std::vector<void *> *curPtr = (std::vector<void *> *) curRawPtr;

			for (size_t i = 0; i < curPtr->size(); i++){
				setSubDimensionVals(value, curDimIndex + 1, (*curPtr)[i]);
			}
		}
		else{
			std::vector<T> *curPtr = (std::vector<T> *) curRawPtr;

			for (size_t i = 0; i < curPtr->size(); i++){
				(*curPtr)[i] = value;
			}
		}
	}

	void *createPrimitiveCopy() const
	{
		void *copy = buildSubDimension(0);
		copySubDimension(0, data, copy);

		return copy;
	}

	T *getIndexPtr(const std::vector<int> &indices) const
	{
		if (isIndicesValid(indices)){
			std::vector<void*> *curPtr = (std::vector < void* >*)data;
			int curDimIndex = 0;

			while (isPtrDim(curDimIndex)){
				int curIndex = indices[curDimIndex];
				void *curRawPointer = (*curPtr)[curIndex];

				curPtr = (std::vector<void*> *) curRawPointer;
				curDimIndex++;
			}
			int curIndex = indices[curDimIndex];

			std::vector<T> *finalDimPtr = (std::vector < T >*) curPtr;
			return &((*finalDimPtr)[curIndex]);
		}

		return NULL;
	}

	T &getIndexQuick(int index, const std::vector<int> &indices) const
	{
		std::vector<void*> *curPtr = (std::vector<void*> *)data;

		int curIndex, curDimIndex;

		if (isMultidim()){
			void *curRawPointer = (*curPtr)[index];
			curPtr = (std::vector<void*> *) curRawPointer;
			curDimIndex = 1;

			while (isPtrDim(curDimIndex)){
				int curIndex = indices[curDimIndex];
				curRawPointer = (*curPtr)[curIndex];

				curPtr = (std::vector<void*> *) curRawPointer;
				curDimIndex++;
			}
			curIndex = indices[curDimIndex];
		}
		else{
			curIndex = index;
		}

		std::vector<T> *finalDimPtr = (std::vector<T> *)curPtr;
		return (*finalDimPtr)[curIndex];
	}

	inline void swapQuick(int index1, int index2, const std::vector<int> &indices)
	{
		T &a = getIndexQuick(index1, indices);
		T &b = getIndexQuick(index2, indices);

		T tmp = a;
		a = b;
		b = tmp;
	}

	void quickSort(const std::vector<int> &indices, std::function<bool(const T&, const T&)> cmp_cb, int left, int right) {

		int i = left, j = right;
		T pivot = getIndexQuick((left + right) / 2, indices);

		while (i <= j) {
			while (cmp_cb(getIndexQuick(i, indices), pivot))
				i++;

			while (cmp_cb(pivot, getIndexQuick(j, indices)))
				j--;

			if (i <= j) {
				swapQuick(i, j, indices);
				i++;
				j--;
			}
		}

		if (left < j)
			quickSort(indices, cmp_cb, left, j);

		if (i < right)
			quickSort(indices, cmp_cb, i, right);
	}
};