// This is a tutorial file. Feel free to remove it.

#include "PolarCoordinates.hpp"

namespace Petr_Math {
	void PolarCoordinates::Normalize()
	{
		angle = fmod(angle, 360.0f);
	}

	PolarCoordinates::PolarCoordinates(float radius, float angleInDegrees)
	{
		this->radius = radius;
		this->angle = angleInDegrees;
	}

	PolarCoordinates::PolarCoordinates(Petr_Math::Vector CartesianCords)
	{
		//r = sqrt(x^2 + z^2)
		radius = sqrt(CartesianCords[0] * CartesianCords[0] + CartesianCords[2] * CartesianCords[2]);
		//z, x
		angle = atan2(CartesianCords[2], CartesianCords[0]);
	}

	Petr_Math::Vector PolarCoordinates::toCartesian()
	{
		Normalize();
		float x = radius * cos(angle);
		float z = radius * sin(angle);
		return Petr_Math::Vector(x, 0.0f, z);
	}
}