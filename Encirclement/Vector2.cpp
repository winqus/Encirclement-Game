#include "Vector2.h"

namespace enc {
	/* Vector2 */
	Vector2::Vector2() : x(0.0), y(0.0) {}
	Vector2::Vector2(float x, float y) : x(x), y(y) {}
	float Vector2::magnitude() const {
		return (float)sqrt(pow(x, 2) + pow(y, 2));
	}
	float Vector2::distance(Vector2 vec2) const {
		return (float)sqrt(pow(x - vec2.x, 2) + pow(y - vec2.y, 2));
	}
	Vector2 Vector2::normalize() {
		float mag = magnitude();
		x /= mag;
		y /= mag;
		return *this;
	}
	float Vector2::dot(Vector2 vec2) {
		return (x * vec2.x + y * vec2.y);
	}
	Vector2 Vector2::rotate(float degrees) {
		float radians = DegToRad(degrees);
		Vector2 vec = *this;
		this->x = vec.x * cos(radians) - vec.y * sin(radians);
		this->y = vec.x * sin(radians) + vec.y * cos(radians);
		return *this;
	}
	Vector2 Vector2::operator+ (Vector2 vec2) {
		return Vector2(x + vec2.x, y + vec2.y);
	}
	Vector2 Vector2::operator- (Vector2 vec2) {
		return Vector2(x - vec2.x, y - vec2.y);
	}
	void Vector2::operator= (Vector2 vec2) {
		x = vec2.x;
		y = vec2.y;
	}
	float Vector2::operator* (Vector2 vec2) {
		return dot(vec2);
	};
	Vector2 Vector2::operator* (float scalar) {
		return Vector2(x * scalar, y * scalar);
	};

	const Vector2 Vector2::Zero = Vector2(0.0f, 0.0f);
	const Vector2 Vector2::Up = Vector2(0.0f, 1.0f);
	const Vector2 Vector2::Down = Vector2(0.0f, -1.0f);
	const Vector2 Vector2::Left = Vector2(1.0f, 0.0f);
	const Vector2 Vector2::Right = Vector2(-1.0f, 0.0f);
}