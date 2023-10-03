// This is a tutorial file. Feel free to remove it.

#include "Quaternion.hpp"


namespace Petr_Math {
	Quaternion::Quaternion(float scalar, Vector v) : s(scalar), u(v)
	{
	}

	Quaternion Quaternion::Conjugation()
	{
		Quaternion result(s, u.opposite());
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

	Quaternion Quaternion::Normalize()
	{
		float len = this->Length();
		float newS = s / len;
		Quaternion newQ(newS, u/len);
		return newQ;
	}

	Quaternion Quaternion::slerp(Quaternion q2, float t)
	{
		Quaternion q1 = Normalize();
		Quaternion q2 = q2.Normalize();
		float alpha = acos(q1.s * q2.s + q1.u.dot(q2.u));
		Vector a = (q2.u * q1.s - q1.u * q2.s + q1.u.cross(q2.u))/sin(alpha);
		Quaternion res(cos(t*alpha), a * sin(t*alpha));
		return res * q1;
	}

	AxisAngle Quaternion::ToAxisAngle()
	{
		return AxisAngle();
	}
	Quaternion Quaternion::operator*(Quaternion q2)
	{
		float newS = s * q2.s - u.dot(q2.u);
		Vector newVec = q2.u * s + u * q2.s + u.cross(q2.u);
		return Quaternion(newS, newVec);
	}
}