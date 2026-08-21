#include "WSEBitStream.h"
#include "WSE.h"

WSEBitStream::WSEBitStream()
{
	m_cursor = 0;
	m_total = 0;
	m_buffer = 0;
	m_mask_table[0] = 0;
	m_delta_last = 0;
	m_fileBufferPos = 0;

	for (int i = 0; i < 32; ++i)
	{
		m_mask_table[i + 1] = m_mask_table[i] | (1 << i);
	}
}

WSEBitStream::~WSEBitStream()
{
	// Close() is the normal path and flushes already; this only covers a stream that
	// is torn down without it, so a capture never loses its tail.
	if (m_stream.is_open())
		FlushFileBuffer();
}

bool WSEBitStream::Open(const char *path)
{
	m_stream.open(path, std::ios::trunc|std::ios::binary);
	return m_stream.is_open();
}

bool WSEBitStream::IsOpen() const
{
	return m_stream.is_open();
}

unsigned __int64 WSEBitStream::Length()
{
	return m_total;
}

void WSEBitStream::Close()
{
	Commit(true);
	FlushFileBuffer();
	m_stream.close();
}

void WSEBitStream::Flush()
{
	FlushFileBuffer();
	m_stream.flush();
}

void WSEBitStream::FlushFileBuffer()
{
	if (m_fileBufferPos)
	{
		m_stream.write(m_fileBuffer, m_fileBufferPos);
		m_fileBufferPos = 0;
	}
}

void WSEBitStream::Commit(bool force)
{
	if (m_cursor == 0 || (m_cursor != 32 && !force))
		return;

	if (m_fileBufferPos > WSE_BIT_STREAM_BUFFER_SIZE - 4)
		FlushFileBuffer();

	memcpy(&m_fileBuffer[m_fileBufferPos], &m_buffer, 4);
	m_fileBufferPos += 4;
	m_total += 32 - m_cursor;
	m_cursor = 0;
	m_buffer = 0;
}

//First writes into 32bit buffer (cursor is current pos in it), once full commit() will write it to stream.
void WSEBitStream::WriteU32(unsigned int value, size_t size)
{
	assert(size <= 32);

	if (size <= 0)
		return;

	if (m_cursor + size > 32)
	{
		int size_1 = 32 - m_cursor;
		int size_2 = size - size_1;

		WriteU32(value, size_1);
		WriteU32(value >> size_1, size_2);
		return;
	}
	
	m_buffer |= ((value & m_mask_table[size]) << m_cursor);
	m_cursor += size;
	m_total += size;
	Commit();
}

void WSEBitStream::WriteU64(unsigned __int64 value, size_t size)
{
	if (size <= 32)
	{
		WriteU32((unsigned int)value, size);
	}
	else
	{
		WriteU32((unsigned int)value, 32);
		WriteU32((unsigned int)(value >> (size - 32)), size - 32);
	}
}

//write as base 15, 15 (0xF) will signal stop (no fixed length)
void WSEBitStream::WriteBCI15(unsigned int value)
{
	int det = 1;

	while (value)
	{
		int mod = value % (det * 15);

		WriteU32(mod / det, 4);

		value -= mod;
		det *= 15;
	}

	WriteU32(15, 4);
}

void WSEBitStream::WriteBCI15(ULONGLONG value)
{
	ULONGLONG det = 1;

	while (value)
	{
		ULONGLONG mod = value % (det * 15);

		WriteU32((unsigned int)(mod / det), 4);

		value -= mod;
		det *= 15;
	}

	WriteU32(15, 4);
}

void WSEBitStream::Write_DeltaBCI15(LONGLONG value)
{
	if(m_delta_last > value)
	{
		WriteU32(1, 1);
		WriteBCI15((ULONGLONG)(m_delta_last - value));
	}
	else
	{
		WriteU32(0, 1);
		WriteBCI15((ULONGLONG)(value - m_delta_last));
	}
	m_delta_last = value;
}

void WSEBitStream::WriteString(const rgl::string &value)
{
	WriteU32(value.length(), 12);

	for (int i = 0; i < value.length(); ++i)
	{
		WriteU32(value[i], 8);
	}
}
