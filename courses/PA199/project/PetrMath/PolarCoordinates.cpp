// This is a tutorial file. Feel free to remove it.

#include "PolarCoordinates.hpp"

#define M_PI       3.14159265358979323846   // pi
#define R2D 180.0f/M_PI
#define D2R M_PI/180.0f

namespace Petr_Math {
	void PolarCoordinates::Normalize()
	{
		if (angle < 0.0f)
		{
			angle = 360.0f + angle;
		}
		angle = fmodf(angle, 360.0f);
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
		
		// y/x in degrees
		angle = atan2(CartesianCords[1], CartesianCords[0]) * R2D;
		Normalize();
	}

	Petr_Math::Vector PolarCoordinates::toCartesian()
	{
		Normalize();
		float x = radius * cos(angle * D2R);
		float y = radius * sin(angle * D2R);
		return Petr_Math::Vector(x, y, 0.0f);
	}
	bool PolarCoordinates::operator==(PolarCoordinates PC)
	{
		if (PC.radius == radius && PC.angle == angle)
		{
			return true;
		}
		return false;
	}
}