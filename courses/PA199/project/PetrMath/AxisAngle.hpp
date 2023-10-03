// This is a tutorial file. Feel free to remove it.
#include <cmath>

#pragma once

namespace Petr_Math {

	class AxisAngle {
	public:
		float angle;
		Vector axis;
		void copy(AxisAngle const& axisA);
		AxisAngle(float a, Vector v);
		~AxisAngle();
		Quaternion ToQuaternion();
	};
}