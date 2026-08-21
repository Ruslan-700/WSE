#pragma once

#include <fstream>
#include <string>
#include "warband.h"

// The bitstream commits one 32 bit word at a time, so writing straight to the stream
// meant an ofstream::write call per four bytes from the game thread. Words are staged
// here first and handed over in one call per full block; the byte sequence on disk is
// unchanged.
#define WSE_BIT_STREAM_BUFFER_SIZE 65536

class WSEBitStream
{
public:
	WSEBitStream();
	~WSEBitStream();
	bool Open(const char *path);
	bool IsOpen() const;
	unsigned __int64 Length();
	void Close();
	void Flush();
	void Commit(bool force = false);
	void WriteU32(unsigned int value, size_t size);
	void WriteU64(unsigned __int64 value, size_t size);
	void WriteBCI15(unsigned int value);
	void WriteBCI15(ULONGLONG value);
	void WriteString(const rgl::string &value);
	void Write_DeltaBCI15(LONGLONG value);

private:
	void FlushFileBuffer();

private:
	std::ofstream m_stream;
	unsigned int m_cursor;
	unsigned int m_buffer;
	unsigned int m_mask_table[33];
	unsigned __int64 m_total;
	LONGLONG m_delta_last;
	char m_fileBuffer[WSE_BIT_STREAM_BUFFER_SIZE];
	int m_fileBufferPos;
};
