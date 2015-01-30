#pragma once

#include "rgl_string.h"

namespace rgl
{
	struct io_stream
	{
		string path;
		int u1;
		bool auto_flush;
		int cursor;
		bool binary;
		string contents;

		virtual void clear();
		virtual void reset();
		virtual io_stream &write_stream(const io_stream &value);
		virtual io_stream &write_float(const float &value);
		virtual io_stream &write_bool(const bool &value);
		virtual io_stream &write_uint64(const unsigned __int64 &value);
		virtual io_stream &write_int64(const __int64 &value);
		virtual io_stream &write_uint32(const unsigned int &value);
		virtual io_stream &write_int32(const int &value);
		virtual io_stream &write_byte(const byte &value);
		virtual io_stream &write_c_str(const char *value);
		virtual io_stream &write_string(const string &value);
		virtual io_stream &unknown_1();
		virtual io_stream &read_stream(io_stream &value);
		virtual io_stream &read_float(float &value);
		virtual io_stream &read_bool(bool &value);
		virtual io_stream &read_uint32(unsigned int &value);
		virtual io_stream &read_uint64(unsigned __int64 &value);
		virtual io_stream &read_int64(__int64 &value);
		virtual io_stream &read_int32(int &value);
		virtual io_stream &read_byte(byte &value);
		virtual io_stream &read_string(string &value);
		virtual io_stream &unknown_2();
		virtual bool at_end() const;
		virtual void set_auto_flush(bool value);
		virtual bool get_auto_flush() const;
		virtual int flush();
	};
}
