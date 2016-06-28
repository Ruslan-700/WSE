#include <vector>

class WSESimpleCharBuff
{
public:
	WSESimpleCharBuff()
	{
		flatDataPtr = NULL;
	}

	~WSESimpleCharBuff()
	{
		if (flatDataPtr != NULL)
			delete[] flatDataPtr;
	}

	void write(const char *dataPtr, int size)
	{
		for (int i = 0; i < size; i++)
			buffer.push_back(*(dataPtr + i));
	}

	char *getDataPtr()
	{
		if (flatDataPtr != NULL)
			delete[] flatDataPtr;

		if (buffer.size() <= 0)
			return NULL;

		flatDataPtr = new char[buffer.size()];

		for (size_t i = 0; i < buffer.size(); i++)
			*(flatDataPtr + i) = buffer[i];

		return flatDataPtr;
	}

	int size() const
	{
		return buffer.size();
	}

	void clear()
	{
		buffer.clear();

		if (flatDataPtr != NULL){
			delete[] flatDataPtr;
			flatDataPtr = NULL;
		}
	}

protected:
	std::vector<char> buffer;
	char *flatDataPtr;
};