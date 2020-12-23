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

void game_memcpy_s(void *dest, const unsigned char *src, int num_bytes)
{
	DWORD prot_old, prot_new;

	VirtualProtect(dest, num_bytes, PAGE_EXECUTE_READWRITE, &prot_old);
	memcpy_s(dest, num_bytes, src, num_bytes);
	VirtualProtect(dest, num_bytes, prot_old, &prot_new);
}

const char HEX2DEC[256] =
{
	/*       0  1  2  3   4  5  6  7   8  9  A  B   C  D  E  F */
	/* 0 */ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	/* 1 */ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	/* 2 */ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	/* 3 */  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, -1, -1, -1, -1, -1, -1,

	/* 4 */ -1, 10, 11, 12, 13, 14, 15, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	/* 5 */ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	/* 6 */ -1, 10, 11, 12, 13, 14, 15, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	/* 7 */ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,

	/* 8 */ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	/* 9 */ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	/* A */ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	/* B */ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,

	/* C */ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	/* D */ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	/* E */ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	/* F */ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1
};

std::string UriDecode(const std::string & sSrc)
{
	// Note from RFC1630:  "Sequences which start with a percent sign
	// but are not followed by two hexadecimal characters (0-9, A-F) are reserved
	// for future extension"

	const unsigned char * pSrc = (const unsigned char *)sSrc.c_str();
	const int SRC_LEN = sSrc.length();
	const unsigned char * const SRC_END = pSrc + SRC_LEN;
	const unsigned char * const SRC_LAST_DEC = SRC_END - 2;   // last decodable '%' 

	char * const pStart = new char[SRC_LEN];
	char * pEnd = pStart;

	while (pSrc < SRC_LAST_DEC)
	{
		if (*pSrc == '%')
		{
			char dec1, dec2;
			if (-1 != (dec1 = HEX2DEC[*(pSrc + 1)])
				&& -1 != (dec2 = HEX2DEC[*(pSrc + 2)]))
			{
				*pEnd++ = (dec1 << 4) + dec2;
				pSrc += 3;
				continue;
			}
		}

		*pEnd++ = *pSrc++;
	}

	// the last 2- chars
	while (pSrc < SRC_END)
		*pEnd++ = *pSrc++;

	std::string sResult(pStart, pEnd);
	delete[] pStart;
	return sResult;
}

// Only alphanum is safe.
const char SAFE[256] =
{
	/*      0 1 2 3  4 5 6 7  8 9 A B  C D E F */
	/* 0 */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	/* 1 */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	/* 2 */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	/* 3 */ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0,

	/* 4 */ 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	/* 5 */ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0,
	/* 6 */ 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	/* 7 */ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0,

	/* 8 */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	/* 9 */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	/* A */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	/* B */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

	/* C */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	/* D */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	/* E */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	/* F */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

std::string UriEncode(const std::string & sSrc)
{
	const char DEC2HEX[16 + 1] = "0123456789ABCDEF";
	const unsigned char * pSrc = (const unsigned char *)sSrc.c_str();
	const int SRC_LEN = sSrc.length();
	unsigned char * const pStart = new unsigned char[SRC_LEN * 3];
	unsigned char * pEnd = pStart;
	const unsigned char * const SRC_END = pSrc + SRC_LEN;

	for (; pSrc < SRC_END; ++pSrc)
	{
		if (SAFE[*pSrc])
			*pEnd++ = *pSrc;
		else
		{
			// escape this char
			*pEnd++ = '%';
			*pEnd++ = DEC2HEX[*pSrc >> 4];
			*pEnd++ = DEC2HEX[*pSrc & 0x0F];
		}
	}

	std::string sResult((char *)pStart, (char *)pEnd);
	delete[] pStart;
	return sResult;
}

std::string replaceString(std::string subject, std::string const & search, std::string const & replace) 
{
	size_t pos = 0;
	while ((pos = subject.find(search, pos)) != std::string::npos) {
		subject.replace(pos, search.length(), replace);
		pos += replace.length();
	}
	return subject;
}

int rglMod(int a, int b)
{
	return b > 1 ? (a % b) : 0;
}

float rglMin(float a, float b)
{
	return a <= b ? a : b;
}

int rglMin(int a, int b)
{
	return a <= b ? a : b;
}

int rglMin(unsigned int a, unsigned int b)
{
	return a <= b ? a : b;
}

float rglMax(float a, float b)
{
	return a >= b ? a : b;
}

int rglMax(int a, int b)
{
	return a >= b ? a : b;
}

unsigned int rglMax(unsigned int a, unsigned int b)
{
	return a >= b ? a : b;
}

float rglClamp(float value, float min, float max)
{
	if (value < min)
		return min;

	if (value > max)
		return max;

	return value;
}

void rglSrand(int seed)
{
	srand(seed);
}

float rglRandf()
{
	return rglRand() % 15817 / 15817.0f;
}

float rglRandf(float max)
{
	return rglRandf() * max;
}

float rglRandf(float min, float max)
{
	return rglRandf() * (max - min) + min;
}

int rglRand()
{
	return rand();
}

int rglRand(int max)
{
	return max > 1 ? (rglRand() % max) : 0;
}

int rglRand(int min, int max)
{
	return rglRand(max - min) + min;
}

bool rglBetween(int value, int min, int max)
{
	return value >= min && value < max;
}

bool rglBetween(float value, float min, float max)
{
	return value >= min && value < max;
}

bool rglBetweenInclusive(int value, int min, int max)
{
	return value >= min && value <= max;
}

bool rglBetweenInclusive(float value, float min, float max)
{
	return value >= min && value <= max;
}

float rglLog2(float value)
{
	return std::log10(value) / RGL_LOG2;
}

float rglAbs(float value)
{
	return std::abs(value);
}

int rglAbs(int value)
{
	return std::abs(value);
}

float rglSqrt(float value)
{
	return std::sqrt(value);
}

float rglPow(float value, float exponent)
{
	return std::pow(value, exponent);
}

float rglSin(float value)
{
	return std::sin(value);
}

float rglCos(float value)
{
	return std::cos(value);
}

float rglTan(float value)
{
	return std::tan(value);
}

float rglAsin(float value)
{
	return std::asin(value);
}

float rglAcos(float value)
{
	return std::acos(value);
}

float rglAtan(float value)
{
	return std::atan(value);
}

float rglAtan2(float y, float x)
{
	return std::atan2(y, x);
}

float rglSquare(float value)
{
	return value * value;
}

int rglSquare(int value)
{
	return value * value;
}

int rglRound(float value)
{
	float f = floor(value);

	return (value - f > 0.5f) ? (int)f + 1 : (int)f;
}

int rglRoundAway(float value)
{
	if (value <= 0.0f)
		return (int)ceil(value - 0.00000001f);
	else
		return (int)ceil(value + 0.00000001f);
}

__int64 rglRound64(float value)
{
	float f = floor(value);

	return (value - f > 0.5f) ? (__int64)f + 1 : (__int64)f;
}

float rglDegToRad(float degrees)
{
	return degrees * 0.0174532925f;
}

float rglRadToDeg(float radians)
{
	return radians * 57.2957795f;
}
