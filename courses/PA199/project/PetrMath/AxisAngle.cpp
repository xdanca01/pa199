// This is a tutorial file. Feel free to remove it.

#include "AxisAngle.hpp"

namespace Petr_Math {
		void AxisAngle::copy(AxisAngle const& axisA)
		{
			angle = axisA.angle;
			axis = axisA.axis;
		}
		AxisAngle::AxisAngle(float a, Vector v) : axis(v), angle(a)
		{

		}
		AxisAngle::~AxisAngle()
		{
		}
		Quaternion AxisAngle::ToQuaternion()
		{
			return Quaternion();
		}
}