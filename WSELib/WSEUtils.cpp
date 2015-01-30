#include "WSEUtils.h"

std::string &trim(std::string &str, const std::string &chars)
{
	ltrim(str, chars);
	rtrim(str, chars);
	return str;
}

std::string &ltrim(std::string &str, const std::string &chars)
{
	int len = str.length();

	for (int i = 0; i < len; ++i)
	{
		if (chars.find(str[i]) == std::string::npos)
		{
			str.erase(0, i);
			break;
		}
	}

	return str;
}

std::string &rtrim(std::string &str, const std::string &chars)
{
	int len = str.length();

	for (int i = len - 1; i >= 0; --i)
	{
		if (chars.find(str[i]) == std::string::npos)
		{
			str.erase(i + 1);
			break;
		}
	}

	return str;
}

std::string &spaces_to_underscores(std::string &str)
{
	int len = str.length();

	for (int i = 0; i < len; ++i)
	{
		if (str[i] == ' ')
			str[i] = '_';
	}

	return str;
}

std::string itostr(int value, int radix)
{
	char buf[16];

	_itoa_s(value, buf, radix);

	return buf;
}

std::string ltostr(int value, int radix)
{
	char buf[32];

	_i64toa_s(value, buf, 32, radix);

	return buf;
}

rgl::string format(const char *format, ...)
{
	va_list ap;
	char buffer[2048];

	va_start(ap, format);
	vsprintf_s(buffer, format, ap);
	va_end(ap);
	return buffer;
}
