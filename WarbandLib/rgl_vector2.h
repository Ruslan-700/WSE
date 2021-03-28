#pragma once

namespace rgl
{
	struct vector2
	{
		float x;
		float y;
		
		vector2(float f = 0.0f);
		vector2(float x, float y);
		float dot(const vector2 &v) const;
		float length() const;
		float length_squared() const;
		float normalize();
		vector2 normalized();
		operator float *();
		operator float *() const;
	};
}
