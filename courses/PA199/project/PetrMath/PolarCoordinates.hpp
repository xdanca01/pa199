// This is a tutorial file. Feel free to remove it.
#include <cmath>
#include "Vector.hpp"
//#include "Quaternion.hpp"

#pragma once

namespace Petr_Math {

	class PolarCoordinates {
	public:
		float radius;
		float angle;
		void Normalize();
		PolarCoordinates(float radius, float angleInDegrees);
		PolarCoordinates(Petr_Math::Vector CartesianCords);
		Petr_Math::Vector toCartesian();
	};
}