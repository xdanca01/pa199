// This is a tutorial file. Feel free to remove it.

#include "Quaternion.hpp"

namespace Petr_Math {
	Quaternion::Quaternion(float scalar, Vector v) : s(scalar), u(v)
	{
	}
	Quaternion Quaternion::Conjugation()
	{
		Quaternion result = new Quaternion(s, u.opposite());
		return result;
	}
	float Quaternion::Length()
	{
		Quaternion res = this->Conjugation();
		return sqrt(s * s + u.dot(res.u));
	}
	Quaternion Quaternion::Inverse()
	{
		Quaternion res = this->Conjugation();
		float length = this->Length();
		if (length == 1) {
			return res;
		}
		res = res * (1 / (length * length));
		return res;
	}
	Quaternion Quaternion::operator*(float num)
	{
		Quaternion res = Quaternion(s, u);
		res.s = s * num;
		res.u = u * num;
		return res;
	}
	float Quaternion::dot(Quaternion p)
	{
		return s * p.s + u.dot(p.u);
	}
	Quaternion Quaternion::slerp(Quaternion q2, float t)
	{
		float sNew = (1 - t) * s + t * q2.s;
		float cosTheta = u.dot(q2.u) / (u.magnitude * q2.u.magnitude);
		Vector vecNew = sin(1-t) * 
		Quaternion res = Quaternion()
		
	}
	AxisAngle Quaternion::ToAxisAngle()
	{
		return AxisAngle();
	}
}