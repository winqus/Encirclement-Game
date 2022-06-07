#include "MathUtilities.h"

namespace enc {
	float DegToRad(const float degrees) {
		return degrees * (float)PI / 180.0f;
	}
	float RadToDeg(const float radians) {
		return radians * 180.0f / (float)PI;
	}
	float Circumference(const float radius) {
		return 2 * (float)PI * radius;
	}
	float RandomNumber(float min_inclusive, float max_inclusive, int min_precision) {
		if (min_inclusive < 0.0f || max_inclusive < 0.0f)
			return 0.0f;
		if (min_inclusive >= max_inclusive)
			return max_inclusive;

		int length = 0;
		float min_num = min_inclusive;
		float max_num = max_inclusive;
		while (max_num-(int)max_num > 0.001f || length < min_precision) {
			max_num *= 10;
			min_num *= 10;
			length++;
		}
		int n = rand() % (int)(max_num - min_num) + (int)min_num;
		float ret = (float)n / (float)std::pow(10, length);

		return ret;
	}
}