#pragma once
#include <cmath>
#include "MathUtilities.h"

namespace enc {
	class Vector2 {
	public:
		float x, y;
		Vector2();
		Vector2(float x, float y);
		float magnitude() const;
		float distance(Vector2 vec2) const;
		Vector2 normalize();
		float dot(Vector2 vec2);
		Vector2 rotate(float degrees);

		Vector2 operator+ (Vector2 vec2);
		Vector2 operator- (Vector2 vec2);
		void operator= (Vector2 vec2);
		float operator* (Vector2 vec2);
		Vector2 operator* (float scalar);

		static const Vector2 Zero;
		static const Vector2 Up;
		static const Vector2 Down;
		static const Vector2 Left;
		static const Vector2 Right;
	};

}

