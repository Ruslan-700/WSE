#include <vector>

//base array -> vector ???
template <type T>
class WSEDynMultiArray
{
public:
	WSEDynNDimArray(const std::vector<int> &dimensions)
	{
		dimCount = dimensions.size();

		if (dimCount < 1)
			return;

		dims = new int[dimCount];
		for (int i = 0; i < dimCount; i++){
			if (dimensions[i] < 1){
				delete[] dims;
				return;
			}
			dims[i] = dimensions[i];
		}

		dimOffsetFactors = new int[dimCount];
		setOffsetFactors();

		int size = getArrayProduct(dims, dimCount);
		data = new T[size]();

		int a[5];
		int *b;

		int c;
		c = &a;
	}

	~WSEDynNDimArray()
	{
		if (dimCount > 0){
			delete[] data;
			delete[] dims;
			delete[] dimOffsetFactors;
		}
	}

	bool setIndex(const std::vector<int> &indices, T value)
	{
		int address = getIndexAdress(indices);

		if (address == -1){
			return false;
		}
		else{
			data[address] = value;
			return true;
		}
	}

	T* getIndex(const std::vector<int> &indices)
	{
		int address = getIndexAdress(indices);

		if (address == -1){
			return NULL;
		}
		else{
			return &data[address];
		}
	}

	int getDimSize(int dimension)
	{
		if ((dimension >= 0) && (dimension <= dimCount)){
			return dims[dimension];
		}
		return 1;
	}

protected:
	T *data;

	int *dims;
	int *dimOffsetFactors;

	std::size_t dimCount;

	int getArrayProduct(int *arr, int arrSize)
	{
		int res = arr[0];
		for (int i = 1; i < arrSize; i++){
			res *= arr[i];
		}
		return res;
	}

	void setOffsetFactors()
	{
		int i = dimCount - 1;
		int curFactor = 1;

		dimOffsetFactors[i] = 1;
		while (i > 0){
			curFactor *= dims[i--];
			dimOffsetFactors[i] = curFactor;
		}
	}

	int getIndexAdress(const std::vector<int> &indices)
	{
		if (indices.size() != dimCount)
			return -1;

		int address = 0;
		for (int i = 0; i < dimCount; i++){
			if ((indices[i] < 0) || (indices[i] > dims[i]))
				return -1;

			address += indices[i] * dimOffsetFactors[i];
		}

		return address;
	}
};