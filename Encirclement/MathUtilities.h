#pragma once
#include <algorithm>
#include <cmath>
#include <time.h>
#include <iostream>

#define PI 3.14159265358979323846


namespace enc {
	template <typename T>
	T clamp(const T& value, const T& minimum, const T& maximum) {
		return std::max(minimum, std::min(value, maximum));
	}

	float DegToRad(const float degrees);
	float RadToDeg(const float radians);
	float Circumference(const float radius);

	float RandomNumber(float min_inclusive, float max_inclusive, int min_precision=3);
}

