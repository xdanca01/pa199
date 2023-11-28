// This is a tutorial file. Feel free to remove it.
#include <cmath>
#include "Vector.hpp"
#include "Quaternion.hpp"

#pragma once

namespace Petr_Math {

	class AxisAngle {
	public:
		float angle;
		Vector axis;
		AxisAngle(float a, Vector& v) : axis(v), angle(a) {};
		void copy(AxisAngle const& axisA)
		{
			axis = axisA.axis;
			angle = axisA.angle;
		}
		~AxisAngle() {};
		Quaternion toQuaternion()
		{
			return Quaternion(*this);
		}
	};
}