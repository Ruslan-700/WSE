#include "network_buffer.h"

#include <climits>
#include <cmath>
#include "warband.h"

using namespace wb;

network_buffer::network_buffer()
{
	this->receive_time = 0.0;
	this->u1 = 0;
	this->total_size_in_bits = NETWORK_BUFFER_SIZE_IN_BITS;
	this->external_buffer = nullptr;
	this->buffer = this->data;
	this->cursor_in_bits = 0;
	this->size_in_bits = 0;
}

network_buffer::~network_buffer()
{
	if (this->external_buffer)
		rgl::_delete(this->external_buffer);
}

void network_buffer::require_size(const int &required_size_in_bits)
{
	if (this->total_size_in_bits >= required_size_in_bits)
		return;

	do
	{
		this->total_size_in_bits *= 2;
	}
	while (this->total_size_in_bits < required_size_in_bits);

	unsigned char *new_buffer = rgl::_new<unsigned char>(this->total_size_in_bits / 8);

	memcpy(new_buffer, this->buffer, (this->size_in_bits + 7) / 8 * 8);

	if (this->external_buffer)
		rgl::_delete(this->external_buffer);
	
	this->external_buffer = new_buffer;
	this->buffer = new_buffer;
}

void network_buffer::pack_uint16(unsigned short value, int size, int offset)
{
	unsigned short v = value - offset;

	this->require_size(this->size_in_bits + size);
	warband->basic_game.put_value(this->buffer, this->cursor_in_bits, &v, size);
	this->size_in_bits += size;
}

void network_buffer::pack_int16(short value, int size, int offset)
{
	short v = value - offset;

	this->require_size(this->size_in_bits + size);
	warband->basic_game.put_value(this->buffer, this->cursor_in_bits, &v, size);
	this->size_in_bits += size;
}

void network_buffer::pack_uint32(unsigned int value, int size, int offset)
{
	unsigned int v = value - offset;

	this->require_size(this->size_in_bits + size);
	warband->basic_game.put_value(this->buffer, this->cursor_in_bits, &v, size);
	this->size_in_bits += size;
}

void network_buffer::pack_int32(int value, int size, int offset)
{
	int v = value - offset;

	this->require_size(this->size_in_bits + size);
	warband->basic_game.put_value(this->buffer, this->cursor_in_bits, &v, size);
	this->size_in_bits += size;
}

void network_buffer::pack_uint64(const unsigned __int64 &value, int size)
{
	this->require_size(this->size_in_bits + size);
	warband->basic_game.put_data(this->buffer, this->cursor_in_bits, &value, size);
	this->size_in_bits += size;
}

void network_buffer::pack_float(float value, int size, float offset, float precision)
{
	this->require_size(this->size_in_bits + size);

	if (size == 32)
	{
		warband->basic_game.put_value(this->buffer, this->cursor_in_bits, &value, size);
	}
	else
	{
		int v = round_half_up((value - offset) / precision);

		warband->basic_game.put_value(this->buffer, this->cursor_in_bits, &v, size);
	}

	this->size_in_bits += size;
}

void network_buffer::pack_vector4(const rgl::vector4 &value, int size, float offset, float precision)
{
	this->require_size(this->size_in_bits + 3 * size);

	if (size == 32)
	{
		warband->basic_game.put_data(this->buffer, this->cursor_in_bits, &value, 3 * size);
	}
	else
	{
		int x = (int)((value.x - offset) / precision);
		int y = (int)((value.y - offset) / precision);
		int z = (int)((value.z - offset) / precision);
		
		warband->basic_game.put_value(this->buffer, this->cursor_in_bits, &x, size);
		warband->basic_game.put_value(this->buffer, this->cursor_in_bits, &y, size);
		warband->basic_game.put_value(this->buffer, this->cursor_in_bits, &z, size);
	}

	this->size_in_bits += 3 * size;
}

void network_buffer::pack_matrix(const rgl::matrix &value, const int &transl_bits, const float &transl_off, const float &transl_mult, const int &transf_bits, const float &transf_off, const float &transf_mult)
{
	require_size(this->size_in_bits + transl_bits * 3 + transf_bits * 4);

	rgl::quaternion rotation = value.rot.to_quaternion();
	
	if (transl_bits == 32)
	{
		warband->basic_game.put_data(this->buffer, this->cursor_in_bits, &value.o, transl_bits * 3);
	}
	else
	{
		unsigned int x, y, z;

		x = (unsigned int)((value.o.x - transl_off) / transl_mult);
		y = (unsigned int)((value.o.y - transl_off) / transl_mult);
		z = (unsigned int)((value.o.z - transl_off) / transl_mult);
		
		warband->basic_game.put_value(this->buffer, this->cursor_in_bits, &x, transl_bits);
		warband->basic_game.put_value(this->buffer, this->cursor_in_bits, &y, transl_bits);
		warband->basic_game.put_value(this->buffer, this->cursor_in_bits, &z, transl_bits);
	}

	if (transf_bits == 32)
	{
		warband->basic_game.put_data(this->buffer, this->cursor_in_bits, &rotation, transf_bits * 4);
	}
	else
	{
		unsigned int w, x, y, z;

		w = (unsigned int)((rotation.w - transf_off) / transf_mult);
		x = (unsigned int)((rotation.x - transf_off) / transf_mult);
		y = (unsigned int)((rotation.y - transf_off) / transf_mult);
		z = (unsigned int)((rotation.z - transf_off) / transf_mult);
		
		warband->basic_game.put_value(this->buffer, this->cursor_in_bits, &w, transf_bits);
		warband->basic_game.put_value(this->buffer, this->cursor_in_bits, &x, transf_bits);
		warband->basic_game.put_value(this->buffer, this->cursor_in_bits, &y, transf_bits);
		warband->basic_game.put_value(this->buffer, this->cursor_in_bits, &z, transf_bits);
	}

	this->size_in_bits += transl_bits * 3 + transf_bits * 4;
}

void network_buffer::pack_string(const rgl::string &string, const int &len)
{
	int length = string.length();

	if (length >= len)
	{
		rgl::string subStr = string.substr(0, len - 1);

		length = subStr.length();
		pack_uint16(length, 16, 0);
		require_size(this->size_in_bits + length * 8);
		warband->basic_game.put_data(this->buffer, this->cursor_in_bits, subStr.c_str(), length * 8);
		this->size_in_bits += length * 8;
	}
	else
	{
		pack_uint16(length, 16, 0);
		require_size(this->size_in_bits + length * 8);
		warband->basic_game.put_data(this->buffer, this->cursor_in_bits, string.c_str(), length * 8);
		this->size_in_bits += length * 8;
	}
}

unsigned short network_buffer::extract_uint16(int size, int offset)
{
	unsigned short value = 0;

	if (this->cursor_in_bits + size <= this->size_in_bits)
	{
		warband->basic_game.take_value(this->buffer, this->cursor_in_bits, &value, size, 0);
		value += offset;
	}

	return value;
}

short network_buffer::extract_int16(int size, int offset)
{
	short value = 0;

	if (this->cursor_in_bits + size <= this->size_in_bits)
	{
		if (offset == SHRT_MIN)
		{
			warband->basic_game.take_value(this->buffer, this->cursor_in_bits, &value, size, 16);
		}
		else
		{
			warband->basic_game.take_value(this->buffer, this->cursor_in_bits, &value, size, 0);
			value += offset;
		}
	}

	return value;
}

unsigned int network_buffer::extract_uint32(int size, int offset)
{
	unsigned int value = 0;

	if (this->cursor_in_bits + size <= this->size_in_bits)
	{
		warband->basic_game.take_value(this->buffer, this->cursor_in_bits, &value, size, 0);

		if (offset != INT_MAX)
			value += offset;
	}

	return value;
}

int network_buffer::extract_int32(int size, int offset)
{
	int value = 0;

	if (this->cursor_in_bits + size <= this->size_in_bits)
	{
		if (offset == INT_MIN)
		{
			warband->basic_game.take_value(this->buffer, this->cursor_in_bits, &value, size, 32);
		}
		else
		{
			warband->basic_game.take_value(this->buffer, this->cursor_in_bits, &value, size, 0);
			value += offset;
		}
	}

	return value;
}

unsigned __int64 network_buffer::extract_uint64(int size)
{
	unsigned __int64 value = 0;

	if (this->cursor_in_bits + size <= this->size_in_bits)
	{
		warband->basic_game.take_data(this->buffer, this->cursor_in_bits, &value, size, 0);
	}

	return value;
}

float network_buffer::extract_float(int size, float offset, float precision)
{
	float value = 0.0f;

	if (this->cursor_in_bits + size <= this->size_in_bits)
	{
		if (size == 32)
		{
			warband->basic_game.take_value(this->buffer, this->cursor_in_bits, &value, size, 0);
		}
		else
		{
			int v = 0;

			warband->basic_game.take_value(this->buffer, this->cursor_in_bits, &v, size, 0);
			value = v * precision + offset;
		}
	}

	return value;
}

rgl::vector4 &network_buffer::extract_vector4(rgl::vector4 &value, int size, float offset, float precision)
{
	if (this->cursor_in_bits + 3 * size <= this->size_in_bits)
	{
		if (size == 32)
		{
			warband->basic_game.take_data(this->buffer, this->cursor_in_bits, &value, 3 * size, 0);
		}
		else
		{
			int x = 0;
			int y = 0;
			int z = 0;
			
			warband->basic_game.take_value(this->buffer, this->cursor_in_bits, &x, size, 0);
			warband->basic_game.take_value(this->buffer, this->cursor_in_bits, &y, size, 0);
			warband->basic_game.take_value(this->buffer, this->cursor_in_bits, &z, size, 0);
			value.x = x * precision + offset;
			value.y = y * precision + offset;
			value.z = z * precision + offset;
		}
	}
	else
	{
		value.x = 0.0f;
		value.y = 0.0f;
		value.z = 0.0f;
	}

	return value;
}

rgl::matrix &network_buffer::extract_matrix(rgl::matrix &value, const int &transl_bits, const float &transl_off, const float &transl_mult, const int &transf_bits, const float &transf_off, const float &transf_mult)
{
	if (this->cursor_in_bits + 3 * transl_bits + 4 * transf_bits <= this->size_in_bits)
	{
		if (transl_bits == 32)
		{
			warband->basic_game.take_data(this->buffer, this->cursor_in_bits, &value.o, transl_bits * 3, 0);
		}
		else
		{
			int x = 0;
			int y = 0;
			int z = 0;
			
			warband->basic_game.take_value(this->buffer, this->cursor_in_bits, &x, transl_bits, 0);
			warband->basic_game.take_value(this->buffer, this->cursor_in_bits, &y, transl_bits, 0);
			warband->basic_game.take_value(this->buffer, this->cursor_in_bits, &z, transl_bits, 0);
			value.o.x = x * transl_mult + transl_off;
			value.o.y = y * transl_mult + transl_off;
			value.o.z = z * transl_mult + transl_off;
		}
		
		rgl::quaternion rotation;

		if (transf_bits == 32)
		{
			warband->basic_game.take_data(this->buffer, this->cursor_in_bits, &rotation, transf_bits * 4, 0);
		}
		else
		{
			int w = 0;
			int x = 0;
			int y = 0;
			int z = 0;
			
			warband->basic_game.take_value(this->buffer, this->cursor_in_bits, &w, transf_bits, 0);
			warband->basic_game.take_value(this->buffer, this->cursor_in_bits, &x, transf_bits, 0);
			warband->basic_game.take_value(this->buffer, this->cursor_in_bits, &y, transf_bits, 0);
			warband->basic_game.take_value(this->buffer, this->cursor_in_bits, &z, transf_bits, 0);
			rotation.w = w * transf_mult + transf_off;
			rotation.x = x * transf_mult + transf_off;
			rotation.y = y * transf_mult + transf_off;
			rotation.z = z * transf_mult + transf_off;
		}

		value.rot = rotation.to_orientation();
	}

	return value;
}

void network_buffer::extract_c_str(char *buf, const int &len)
{
	if (this->cursor_in_bits + 16 <= this->size_in_bits)
	{
		int length = extract_uint16(16);

		if (length < len && this->cursor_in_bits + length * 8 <= this->size_in_bits)
		{
			warband->basic_game.take_data(this->buffer, this->cursor_in_bits, buf, length * 8, 0);
			buf[length] = '\0';
		}
	}
}

void network_buffer::extract_string(rgl::string &string, const int &len)
{
	this->extract_c_str(string.get_buffer(len), len);
	string.release_buffer();
}

std::string network_buffer::extract_string(const int &length)
{
	char *buffer = new char[length];

	this->extract_c_str(buffer, length);

	std::string value = buffer;
	delete[] buffer;
	return value;
}
