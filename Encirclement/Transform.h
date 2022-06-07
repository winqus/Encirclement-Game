#pragma once
#include "Vector2.h"

namespace enc {
	class Transform {
	protected:
		Vector2 _position;
		Vector2 _scale;
		Vector2 _direction;
	public:
		Transform();
		Vector2 position() const;
		Vector2 scale() const;
		Vector2 direction() const;
		void position(Vector2 position);
		void scale(Vector2 scale);
		void direction(Vector2 direction);
		void LookAt(Vector2 position);
		float Angle(Vector2 toVector) const;
		float GlobalRotation() const;
		void Translate(Vector2 translation);
		void Rotate(float degrees);
	};
}
