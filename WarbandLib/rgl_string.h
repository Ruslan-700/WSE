#pragma once

#include <atlstr.h>
#include <string>
#include "rgl_memory.h"

namespace rgl
{
	struct string
	{
	public:
		int cursor;
		char *buffer;
		int buffer_length;
		int str_length;
		char static_buffer[128];

	public:
		string();
		string(const string &str);
		string(const char *str);
		~string();
		int length() const;
		bool starts_with(const string &str) const;
		int index_of(char c, int start = 0) const;
		int index_of(const string &str, int start = 0) const;
		string substr(int start, int end) const;
		void initialize();
		void resize(int size);
		void reallocate(int size);
		const char *c_str() const;
		const char *static_c_str() const;
		const char &char_at(int pos) const;
		void underscores_to_spaces();
		void spaces_to_underscores();
		void upper();
		void lower();
		void clear();
		string trimmed() const;
		void widen(CStringW &s);
		void format(const char *format, ...);
		char *get_buffer(int size = -1);
		void release_buffer(int size = -1);
		bool is_integer() const;
		string *split(char separator, int &num_parts) const;
		wchar_t *to_utf8() const;

		const char &operator[](int pos) const;
		string &operator=(const string &str);
		string &operator=(const char *str);
		string &operator=(const std::string &str);
		string &operator=(CStringW &s);
		bool operator==(const char *str) const;
		bool operator!=(const char *str) const;
		bool operator==(const string &str) const;
		bool operator!=(const string &str) const;
		void operator+=(const char &ch);
		void operator+=(const string &str);
		const string operator+(const char &ch) const;
		const string operator+(const string &str) const;
		operator const char *();
	};
}
