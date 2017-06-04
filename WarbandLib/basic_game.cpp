#include "basic_game.h"

#include "warband.h"

using namespace wb;

void basic_game::parse_string(rgl::string &out_str, const rgl::string &in_str, const bool &url_encode)
{
	THISCALL3(addresses::basic_game_ParseString, this, out_str, in_str, url_encode)
}

bool basic_game::is_safe_string(const rgl::string &str)
{
	if (str.length() >= 50)
		return false;

	for (int i = 0; i < str.length(); ++i)
	{
		char c = str[i];

		if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || (c == '_'))
			continue;

		return false;
	}

	return true;
}

bool basic_game::is_multiplayer()
{
	return this->game_type != gt_single;
}

bool basic_game::is_client()
{
	return this->game_type == gt_multi_client;
}

bool basic_game::is_server()
{
	return this->game_type == gt_multi_client_server || this->game_type == gt_multi_server;
}

bool basic_game::is_dedicated_server()
{
	return this->game_type == gt_multi_server;
}

bool basic_game::is_listen_server()
{
	return this->game_type == gt_multi_client_server;
}

void basic_game::put_value(unsigned char *dstBuffer, int &dstCursor, const void *srcValue, int srcSize)
{
	unsigned int *intBuffer = (unsigned int *)dstBuffer;
	int cursorShift = dstCursor & 0x1F;
	int index = dstCursor / 32;

	if (cursorShift)
	{
		int blockSize = srcSize;

		if (srcSize >= 32 - cursorShift)
			blockSize = 32 - cursorShift;

		intBuffer[index] = (*(unsigned int *)srcValue << cursorShift) | intBuffer[index] & ~(bit_mask_table[blockSize] << cursorShift);

		if (blockSize < srcSize)
			intBuffer[index + 1] = ((*(unsigned int *)srcValue & (bit_mask_table[srcSize - blockSize] << blockSize)) >> blockSize) | intBuffer[index + 1] & ~bit_mask_table[srcSize - blockSize];

		dstCursor += srcSize;
	}
	else
	{
		intBuffer[index] = *(unsigned int *)srcValue;
		dstCursor += srcSize;
	}
}

void basic_game::put_data(unsigned char *dstBuffer, int &dstCursor, const void *srcBuffer, int srcSize)
{
	if (dstCursor & 0x7)
	{
		int totalSize = 0;

		while (srcSize > 0)
		{
			int cursorShift = dstCursor & 0x7;
			int blockSize = 8 - cursorShift;
			int sizeShift = totalSize & 0x7;

			if (srcSize < 8 - cursorShift)
				blockSize = srcSize;

			if (blockSize > 8 - sizeShift)
				blockSize = 8 - sizeShift;

			srcSize -= blockSize;
			dstBuffer[dstCursor / 8] = (((bit_mask_table[blockSize] << sizeShift) & ((unsigned char *)srcBuffer)[totalSize / 8]) >> sizeShift << cursorShift) | dstBuffer[dstCursor / 8] & ~((unsigned char)bit_mask_table[blockSize] << cursorShift);
			dstCursor += blockSize;
			totalSize += blockSize;
		}
	}
	else
	{
		int sizeinBytes = srcSize / 8;

		if (srcSize & 0x7)
			sizeinBytes++;

		memcpy(&dstBuffer[dstCursor / 8], srcBuffer, sizeinBytes);
		dstCursor += srcSize;
	}
}

void basic_game::take_value(const unsigned char *srcBuffer, int &srcCursor, void *dstValue, int dstSize, int offsetSize)
{
	unsigned int *intBuffer = (unsigned int *)srcBuffer;
	int cursorShift = srcCursor & 0x1F;
	int cursorIndex = srcCursor / 32;
	int blockSize = dstSize;

	if (dstSize >= 32 - cursorShift)
		blockSize = 32 - cursorShift;

	*(unsigned int *)dstValue = ((intBuffer[cursorIndex] & (bit_mask_table[blockSize] << cursorShift)) >> cursorShift) | *(unsigned int *)dstValue & ~bit_mask_table[blockSize];

	if (blockSize < dstSize)
		*(unsigned int *)dstValue = ((bit_mask_table[dstSize - blockSize] & intBuffer[cursorIndex + 1]) << blockSize) | *(unsigned int *)dstValue & ~(bit_mask_table[dstSize - blockSize] << blockSize);

	if (offsetSize > dstSize && *(unsigned int *)dstValue >> (dstSize - 1))
		*(unsigned int *)dstValue |= bit_mask_table[offsetSize - dstSize] << dstSize;

	srcCursor += dstSize;
}

void basic_game::take_data(const unsigned char *srcBuffer, int &srcCursor, void *dstBuffer, int dstSize, int offsetSize)
{
	int totalSize = 0;
	int sizeIndex = 10000;

	while (totalSize < dstSize)
	{
		int blockSize = dstSize - totalSize;
		int cursorShift = srcCursor & 0x7;
		int sizeShift = totalSize & 0x7;

		sizeIndex = totalSize / 8;

		if (blockSize >= 8 - cursorShift)
			blockSize = 8 - cursorShift;

		if (blockSize >= 8 - sizeShift)
			blockSize = 8 - sizeShift;

		((unsigned char *)dstBuffer)[sizeIndex] = (((bit_mask_table[blockSize] << cursorShift) & srcBuffer[srcCursor / 8]) >> cursorShift << sizeShift) | ((unsigned char *)dstBuffer)[sizeIndex] & ~((unsigned char)bit_mask_table[blockSize] << sizeShift);

		srcCursor += blockSize;
		totalSize += blockSize;
	}

	if (offsetSize > totalSize && ((unsigned char *)dstBuffer)[sizeIndex] >> ((totalSize - 1) & 0x7))
	{
		while (totalSize < offsetSize)
		{
			int blockSize = offsetSize - totalSize;
			int sizeShift = totalSize & 0x7;

			if (blockSize >= 8 - sizeShift)
				blockSize = 8 - sizeShift;

			((unsigned char *)dstBuffer)[totalSize / 8] |= (unsigned char)bit_mask_table[blockSize] << sizeShift;
			totalSize += blockSize;
		}
	}
}
