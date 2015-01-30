#include "rgl_common.h"

#include <cmath>

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
