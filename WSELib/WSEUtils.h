#pragma once

#include <sstream>
#include <string>
#include "stl_vector.h"
#include "rgl.h"

std::string &trim(std::string &str, const std::string &chars = " \t\n\v\f\r");
std::string &ltrim(std::string &str, const std::string &chars = " \t\n\v\f\r");
std::string &rtrim(std::string &str, const std::string &chars = " \t\n\v\f\r");
std::string &spaces_to_underscores(std::string &str);
std::string itostr(int value, int radix = 10);
std::string ltostr(__int64 value, int radix = 10);
rgl::string format(const char *format, ...);

template <typename T>
std::string join(const std::string &separator, T *values, size_t size)
{
	std::stringstream ss;

	for (size_t i = 0; i < size; ++i)
	{
		ss << values[i];

		if (i < size - 1)
			ss << separator;
	}

	return ss.str();
}

template <typename T>
std::string join(const std::string &separator, stl::vector<T> values)
{
	std::stringstream ss;
	size_t size = values.size();

	for (size_t i = 0; i < size; ++i)
	{
		ss << values[i];

		if (i < size - 1)
			ss << separator;
	}

	return ss.str();
}
