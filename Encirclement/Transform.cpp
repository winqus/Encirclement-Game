#include "Transform.h"


namespace enc {
	/* Transform */
	Transform::Transform() {
		_position = Vector2();
		_scale = Vector2(1., 1.);
		_direction = Vector2(0., -1.);
	}
	Vector2 Transform::position() const {
		return _position;
	}
	Vector2 Transform::scale() const {
		return _scale;
	}
	Vector2 Transform::direction() const {
		return _direction;
	}
	void Transform::position(Vector2 position) {
		_position = position;
	}
	void Transform::scale(Vector2 scale) {
		_scale = scale;
	}
	void Transform::direction(Vector2 direction) {
		_direction = direction;
	}
	void Transform::LookAt(Vector2 position) {
		Vector2 vDiff = position - this->_position;
		if (vDiff.magnitude() <= 1.0f)
			return;
		vDiff.normalize();
		this->_direction = vDiff;
	}
	float Transform::Angle(Vector2 toVector) const {

		float angle = (float)std::atan2(
			-(this->_direction.x * toVector.y - this->_direction.y * toVector.x),
			this->_direction.x * toVector.x + this->_direction.y * toVector.y
		);
		return angle * 180.f / (float)PI;
	}
	float Transform::GlobalRotation() const {
		float angle = Angle(Vector2(0, -1));
		if (angle == -0.0f)
			angle = 0.0f;
		return angle < 0.f ? angle + 360.f : angle;
	}
	void Transform::Translate(Vector2 translation) {
		this->_position = _position + translation;
	}
	void Transform::Rotate(float degrees) {
		float radians = DegToRad(degrees);
		Vector2 dir = direction();
		direction(Vector2(
			dir.x * cos(radians) - dir.y * sin(radians),
			dir.x * sin(radians) + dir.y * cos(radians)
		));
	}
}