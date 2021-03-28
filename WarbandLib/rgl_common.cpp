#include "rgl_common.h"

#include "warband.h"

#include <cmath>

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

void rglWarning(const char *format, ...)
{
	va_list ap;
	char message[2048];
	char warningMessage[2100];

	va_start(ap, format);
	vsprintf_s(message, format, ap);
	va_end(ap);
	sprintf_s(warningMessage, "WARNING: %s", message);

	warband->log_stream.write_c_str(warningMessage);
	warband->log_stream.write_c_str("\n");

#if defined WARBAND
	warband->window_manager.display_message(warningMessage, 0xFFFF0000, 0);
#endif
}

int file_get_length(FILE *file)
{
	int cur_pos = ftell(file);

	fseek(file, 0, SEEK_END);

	int end_pos = ftell(file);

	fseek(file, cur_pos, SEEK_SET);

	return end_pos;
}

int round_half_up(const float &value)
{
	float floor = std::floor(value);
	int result = (int)floor;

	if (value - result > 0.5)
		++result;

	return result;
}
