#pragma once

#include <fstream>
#include <string>
#include "warband.h"

class WSEBitStream
{
public:
	WSEBitStream();
	bool Open(const char *path);
	bool IsOpen() const;
	unsigned __int64 Length();
	void Close();
	void Flush();
	void Commit(bool force = false);
	void WriteU32(unsigned int value, size_t size);
	void WriteU64(unsigned __int64 value, size_t size);
	void WriteBCI15(unsigned int value);
	void WriteString(const rgl::string &value);

private:
	std::ofstream m_stream;
	unsigned int m_cursor;
	unsigned int m_buffer;
	unsigned int m_mask_table[33];
	unsigned __int64 m_total;
};
