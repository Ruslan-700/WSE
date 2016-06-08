#include <vector>

enum type_id{
	num = 0,
	str = 1,
	pos = 2
};

template <typename T>
class WSEDynMultiArray2
{
public:
	WSEDynMultiArray2(const std::vector<int> &dimensions, type_id typeID)
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

	~WSEDynMultiArray2()
	{
		if (isDimSizesValid()){
			deleteSubDimension(data, 0);
		}

		delete &dimSizes;
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

	bool resize(size_t size)
	{
		if (size < 1){
			return false;
		}

		if (dimSizes.size() > 1){
			std::vector<void*> *dataPtr = (std::vector<void*> *)data;
			size_t oldSize = dataPtr->size();

			if (oldSize < size){
				dataPtr->resize(size);

				for (int i = (oldSize - 1); i < (int)size; i++){
					(*dataPtr)[i] = buildSubDimension(1);
				}
			}
			else if (oldSize > size){
				for (int i = (oldSize - 1); i >= (int)size; i--){
					deleteSubDimension((*dataPtr)[i], 1);
				}

				dataPtr->resize(size);
			}
		}
		else{
			std::vector<T> *dataPtr = (std::vector<T> *)data;
			dataPtr->resize(size);
		}

		dimSizes[0] = size;
		return true;
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

	type_id getTypeID() const
	{
		return dataTypeID;
	}

private:
	void *data;
	std::vector < int > dimSizes;
	type_id dataTypeID;

	bool isDimSizesValid() const
	{
		if (dimSizes.size() < 1)
			return false;

		for (size_t i = 0; i < dimSizes.size(); i++){
			if (dimSizes[i] < 1)
				return false;
		}

		return true;
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

	void  *buildSubDimension(int dimIndex)
	{
		if ((dimIndex + 1) < (int)dimSizes.size()){
			std::vector < void* > *subDim = new std::vector < void* >;
			subDim->resize(dimSizes[dimIndex]);

			for (int i = 0; i < dimSizes[dimIndex]; i++){
				(*subDim)[i] = buildSubDimension(dimIndex + 1);
			}
			return (void*)subDim;
		}
		else{
			std::vector < T > *subDim = new std::vector < T >;
			subDim->resize(dimSizes[dimIndex]);
			return (void*)subDim;
		}
	}

	void deleteSubDimension(void *dimPtr, int dimIndex)
	{
		if ((dimIndex + 1) < (int)dimSizes.size()){
			std::vector < void* > *subDim = (std::vector < void* >*)dimPtr;

			for (int i = 0; i < dimSizes[dimIndex]; i++){
				deleteSubDimension(subDim, dimIndex + 1);
			}
			delete subDim;
		}
		else{
			std::vector < T > *subDim = (std::vector < T >*)dimPtr;
			delete subDim;
		}
	}

	T *getIndexPtr(const std::vector<int> &indices) const
	{
		if (isIndicesValid(indices)){
			std::vector < void* > *curPtr = (std::vector < void* >*)data;
			int curDimIndex = 0;

			while ((curDimIndex + 1) < (int)dimSizes.size()){
				int curIndex = indices[curDimIndex];
				void *curRawPointer = (*curPtr)[curIndex];

				curPtr = (std::vector < void* >*) curRawPointer;
				curDimIndex++;
			}
			int curIndex = indices[curDimIndex];

			std::vector < T > *finalDimPtr = (std::vector < T >*) curPtr;
			return &((*finalDimPtr)[curIndex]);
		}

		return NULL;
	}
};
