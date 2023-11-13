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
		Normalize();
	}

	PolarCoordinates::PolarCoordinates(Petr_Math::Vector CartesianCords)
	{
		//r = sqrt(x^2 + y^2)
		radius = sqrt(CartesianCords[0] * CartesianCords[0] + CartesianCords[1] * CartesianCords[1]);
		//y, x
		angle = atan2(CartesianCords[1], CartesianCords[0]);
		Normalize();
	}

	Petr_Math::Vector PolarCoordinates::toCartesian()
	{
		Normalize();
		float x = radius * cos(angle);
		float y = radius * sin(angle);
		return Petr_Math::Vector(x, y, 0.0f);
	}
}