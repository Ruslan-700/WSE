#include "rgl_string.h"

#include <cassert>

using namespace rgl;

string::string()
{
	this->initialize();
};

string::string(const string &str)
{
	this->initialize();
	*this = str;
}

string::string(const char *str)
{
	this->initialize();

	int new_len = strlen(str);

	if (new_len)
	{
		this->reallocate(new_len);
		memcpy_s(this->buffer, this->buffer_length, str, new_len + 1);
		this->str_length = new_len;
	}
};

string::~string()
{
	if (this->buffer != this->static_buffer)
		_free(this->buffer);
};

int string::length() const
{
	return this->str_length;
}

bool string::starts_with(const string &str) const
{
	if (str.str_length > str_length)
		return false;

	for (int i = 0; i < str.str_length; ++i)
	{
		if (str.buffer[i] != buffer[i])
			return false;
	}

	return true;
}

int string::index_of(char c, int start) const
{
	for (int i = start; i < str_length; ++i)
	{
		if (buffer[i] == c)
			return i;
	}

	return -1;
}

int string::index_of(const string &str, int start) const
{
	int max = str_length - str.str_length + 1;

	for (int i = start; i < max; ++i)
	{
		int j;

		for (j = 0; j < str.str_length; ++j)
		{
			if (buffer[i + j] != str.buffer[j])
				break;
		}

		if (j == str.str_length)
			return i;
	}

	return -1;
}

string string::substr(int start, int end) const
{
	string substr;

	for (int i = start; i < end; ++i)
	{
		substr += buffer[i];
	}

	return substr;
}

void string::initialize()
{
	this->buffer = this->static_buffer;
	this->buffer_length = 128;
	this->str_length = 0;
	this->static_buffer[0] = '\0';
	this->cursor = 0;
}

void string::resize(int size)
{
	if (size <= 0)
		return;

	if (this->buffer_length > size)
		return;

	unsigned int new_size = 128 * ((size / 128) + 1);
	char *new_buffer = (char *)_alloc(new_size);

	memcpy_s(new_buffer, new_size, this->buffer, this->str_length + 1);

	if (this->buffer != this->static_buffer)
		_free(this->buffer);

	this->buffer_length = new_size;
	this->buffer = new_buffer;
}

void string::reallocate(int size)
{
	if (size <= 0)
		return;
	
	if (this->buffer_length > size)
		return;
	
	unsigned int new_size = 128 * ((size / 128) + 1);
	char *new_buffer = (char *)_alloc(new_size);

	if (this->buffer != this->static_buffer)
		_free(this->buffer);

	this->buffer_length = new_size;
	this->buffer = new_buffer;
	this->clear();
}

const char *string::c_str() const
{
	return this->buffer;
}

const char *string::static_c_str() const
{
	return this->static_buffer;
}

const char &string::char_at(int pos) const
{
	return this->buffer[pos];
}

void string::underscores_to_spaces()
{
	for (int i = 0; i < this->str_length; ++i)
	{
		if (this->buffer[i] == '_')
			this->buffer[i] = ' ';
	}
}

void string::spaces_to_underscores()
{
	for (int i = 0; i < this->str_length; ++i)
	{
		if (this->buffer[i] == ' ')
			this->buffer[i] = '_';
	}
}

void string::upper()
{
	for (int i = 0; i < this->str_length; ++i)
	{
		if (this->buffer[i] >= 'a' && this->buffer[i] <= 'z')
			this->buffer[i] -= 32;
	}
}

void string::lower()
{
	for (int i = 0; i < this->str_length; ++i)
	{
		if (this->buffer[i] >= 'A' && this->buffer[i] <= 'Z')
			this->buffer[i] += 32;
	}
}

void string::clear()
{
	this->str_length = 0;
	this->buffer[0] = '\0';
}

string string::trimmed() const
{
	int start;
	int end;
	
	for (start = 0; start < str_length; ++start)
	{
		if (!iswspace(buffer[start]))
			break;
	}

	for (end = str_length; end > start; --end)
	{
		if (!iswspace(buffer[end - 1]))
			break;
	}

	string str;

	str.resize(end - start);

	for (int i = 0; i < end - start; ++i)
	{
		str.buffer[i] = buffer[start + i];
	}

	str.buffer[end - start] = '\0';
	str.str_length = end - start;
	return str;
}

void string::widen(CStringW &s)
{
	if (this->str_length)
	{
		wchar_t *wbuf = s.GetBuffer(this->str_length);
		int length = MultiByteToWideChar(CP_UTF8, 0, this->buffer, this->str_length, wbuf, this->str_length);
	
		s.ReleaseBuffer(length);
	}
	else
	{
		s.Empty();
	}
}

void string::format(const char *format, ...)
{
	char buf[1024];
	va_list ap;

	va_start(ap, format);
	vsnprintf_s(buf, 1024, format, ap);
	*this = buf;
}

char *string::get_buffer(int size)
{
	if (size > 0 && size > this->buffer_length)
		this->resize(size);

	return this->buffer;
}

void string::release_buffer(int size)
{
	if (size >= 0)
		this->str_length = size;
	else
		this->str_length = strlen(this->buffer);
}

bool string::is_integer() const
{
	string trimmed_str = trimmed();
	
	int start = 0;

	if (trimmed_str.length() > 0 && trimmed_str[0] == '-')
		start = 1;

	for (int i = start; i < trimmed_str.length(); ++i)
	{
		if (!isdigit(trimmed_str[i]))
			return false;
	}

	return true;
}

string *string::split(char separator, int &num_parts) const
{
	num_parts = 0;

	for (int i = 0; i < str_length; ++i)
	{
		if (buffer[i] == separator)
			num_parts++;
	}

	num_parts++;

	string *parts = _new_vec<string>(num_parts);
	int curPart = 0;
	int start = 0;

	for (int i = 0; i < str_length; ++i)
	{
		if (buffer[i] == separator)
		{
			parts[curPart++] = substr(start, i);
			start = i + 1;
		}
	}

	parts[curPart] = substr(start, str_length);
	return parts;
}

wchar_t *string::to_utf8() const
{
	int size = length() + 1;
	wchar_t *buf = _new_vec<wchar_t>(size);

	MultiByteToWideChar(CP_UTF8, 0, buffer, -1, buf, size);
	return buf;
}

const char &string::operator[](int pos) const
{
	return this->buffer[pos];
}

string &string::operator=(const string &str)
{
	int new_len = str.str_length;

	this->reallocate(new_len);

	if (new_len)
		memcpy_s(this->buffer, this->buffer_length, str.buffer, new_len);

	this->str_length = new_len;
	this->buffer[new_len] = '\0';
	this->cursor = str.cursor;
	return *this;
}

string &string::operator=(const char *str)
{
	int new_len = strlen(str);

	this->reallocate(new_len);

	if (new_len)
		memcpy_s(this->buffer, this->buffer_length, str, new_len);

	this->str_length = new_len;
	this->buffer[new_len] = '\0';
	this->cursor = 0;
	return *this;
}

string &string::operator=(const std::string &str)
{
	int new_len = str.length();

	this->reallocate(new_len);

	if (new_len)
		memcpy_s(this->buffer, this->buffer_length, str.c_str(), new_len);

	this->str_length = new_len;
	this->buffer[new_len] = '\0';
	this->cursor = 0;
	return *this;
}

string &string::operator=(CStringW &s)
{ 
	if (s.GetLength() == 0)
	{
		this->clear();
		return *this;
	}

	size_t buf_size = 4 * s.GetLength();
	char *buf = new char[buf_size];
	int length = WideCharToMultiByte(CP_UTF8, 0, s.GetBuffer(), s.GetLength(), buf, buf_size, nullptr, nullptr);

	buf[length] = 0;
	*this = buf;
	delete[] buf;
	return *this;
}

bool string::operator==(const char *str) const
{
	if (!this->str_length)
		return !strlen(str);

	if (!strlen(str))
		return false;

	return !strcmp(this->buffer, str);
}

bool string::operator!=(const char *str) const
{
	return !(*this == str);
}

bool string::operator==(const string &str) const
{
	int my_len = this->str_length;
	int len = str.str_length;

	if (!my_len && !len)
		return true;

	if (my_len != len)
		return false;

	return !strcmp(this->buffer, str.buffer);
}

bool string::operator!=(const string &str) const
{
	return !(*this == str);
}

void string::operator+=(const char &str)
{
	this->resize(this->str_length + 1);
	this->buffer[this->str_length++] = str;
	this->buffer[this->str_length] = '\0';
}

void string::operator+=(const string &str)
{
	int new_len = str_length + str.str_length;

	this->resize(new_len);
	memcpy_s(&this->buffer[this->str_length], this->buffer_length - this->str_length, str.buffer, str.str_length + 1);
	this->str_length = new_len;
}

const string string::operator+(const char &str) const
{
	string res = *this;

	res += str;

	return res;
}

const string string::operator+(const string &str) const
{
	string res = *this;

	res += str;

	return res;
}

string::operator const char *()
{
	return this->buffer;
}