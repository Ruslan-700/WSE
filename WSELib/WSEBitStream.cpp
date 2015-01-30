#include "WSEBitStream.h"

WSEBitStream::WSEBitStream()
{
	m_cursor = 0;
	m_total = 0;
	m_buffer = 0;
	m_mask_table[0] = 0;

	for (int i = 0; i < 32; ++i)
	{
		m_mask_table[i + 1] = m_mask_table[i] | (1 << i);
	}
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
	m_stream.close();
}

void WSEBitStream::Flush()
{
	m_stream.flush();
}

void WSEBitStream::Commit(bool force)
{
	if (m_cursor == 0 || (m_cursor != 32 && !force))
		return;

	m_stream.write((char *)&m_buffer, 4);
	m_total += 32 - m_cursor;
	m_cursor = 0;
	m_buffer = 0;
}

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

void WSEBitStream::WriteString(const rgl::string &value)
{
	WriteU32(value.length(), 12);

	for (size_t i = 0; i < value.length(); ++i)
	{
		WriteU32(value[i], 8);
	}
}
