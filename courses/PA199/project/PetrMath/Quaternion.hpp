// This is a tutorial file. Feel free to remove it.

#pragma once
#include "Vector.hpp"
#include "AxisAngle.hpp"

namespace Petr_Math {

	class Quaternion {
	public:
		float s;
		Vector u;
		Quaternion(float scalar, Vector v);
		Quaternion Conjugation();
		float Length();
		Quaternion Inverse();
		Quaternion Quaternion::Normalize();
		Quaternion operator*(float num);
		float dot(Quaternion p);
		Quaternion slerp(Quaternion q2, float t);
		AxisAngle ToAxisAngle();
		Quaternion operator*(Quaternion q2);
	};
}