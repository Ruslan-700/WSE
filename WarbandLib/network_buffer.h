#pragma once

#include "rgl.h"
#include <string>

#define NETWORK_BUFFER_SIZE_IN_BITS 12000

namespace wb
{
	struct network_buffer
	{
		int u1;
		int total_size_in_bits;
		unsigned char data[NETWORK_BUFFER_SIZE_IN_BITS / 8];
		unsigned char *external_buffer;
		unsigned char *buffer;
		int size_in_bits;
		int cursor_in_bits;
		double receive_time;
		
		network_buffer();
		~network_buffer();
		void require_size(const int &required_size_in_bits);
		void pack_uint16(unsigned short value, int size, int offset = 0);
		void pack_int16(short value, int size, int offset = 0);
		void pack_uint32(unsigned int value, int size, int offset = 0);
		void pack_int32(int value, int size, int offset = 0);
		void pack_uint64(const unsigned __int64 &value, int size);
		void pack_float(float value, int size, float offset, float precision);
		void pack_vector4(const rgl::vector4 &value, int size, float offset, float precision);
		void pack_matrix(const rgl::matrix &value, const int &transl_bits, const float &transl_off, const float &transl_mult, const int &transf_bits, const float &transf_off, const float &transf_mult);
		void pack_string(const rgl::string &str, const int &length);
		unsigned short extract_uint16(int size, int offset = 0);
		short extract_int16(int size, int offset = 0);
		unsigned int extract_uint32(int size, int offset = 0);
		int extract_int32(int size, int offset = 0);
		unsigned __int64 extract_uint64(int size);
		float extract_float(int size, float offset, float precision);
		rgl::vector4 &extract_vector4(rgl::vector4 &value, int size, float offset, float precision);
		rgl::matrix &extract_matrix(rgl::matrix &value, const int &transl_bits, const float &transl_off, const float &transl_mult, const int &transf_bits, const float &transf_off, const float &transf_mult);
		void extract_c_str(char *buf, const int &len);
		void extract_string(rgl::string &str, const int &length);
		std::string extract_string(const int &length);
	};
}