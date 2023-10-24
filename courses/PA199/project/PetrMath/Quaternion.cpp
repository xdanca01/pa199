// This is a tutorial file. Feel free to remove it.

#include "Quaternion.hpp"


namespace Petr_Math {
	class AxisAngle;

	Quaternion::Quaternion(float scalar, Vector v) : s(scalar), u(v)
	{
	}

	Quaternion::Quaternion(AxisAngle& axisAngle) : s(cos(axisAngle.angle/2)), u(axisAngle.axis.size)
	{
		float sins = sin(axisAngle.angle / 2);
		u = axisAngle.axis * sins;
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
		Quaternion res(s, u);
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
		Quaternion q3 = q2.Normalize();
		float alpha = acos(q1.s * q3.s + q1.u.dot(q3.u));
		Vector a = (q3.u * q1.s - q1.u * q3.s + q1.u.cross(q3.u))/sin(alpha);
		Quaternion res(cos(t*alpha), a * sin(t*alpha));
		return res * q1;
	}

	Quaternion Quaternion::operator*(Quaternion q2)
	{
		float newS = s * q2.s - u.dot(q2.u);
		Vector newVec = q2.u * s + u * q2.s + u.cross(q2.u);
		return Quaternion(newS, newVec);
	}

	AxisAngle Quaternion::ToAxisAngle()
	{
		Quaternion normalizedQ = Normalize();
		float Alpha = acos(s);
		float sinAlpha = sin(Alpha);
		AxisAngle aa(Alpha, u / sinAlpha);
		return aa;
	}
	Matrix Quaternion::ToRotationMatrix()
	{
		float data[16] = {0};
		data[0] = 2 * (s * s + u[0] * u[0]) - 1;
		data[1] = 2 * (u[0] * u[1] - s * u[2]);
		data[2] = 2 * (u[0] * u[2] + s * u[1]);

		data[5] = 2 * (u[1] * u[2] + s * u[2]);
		data[6] = 2 * (s * s + u[1] * u[1]) - 1;
		data[7] = 2 * (u[1] * u[2] - s * u[0]);

		data[9] = 2 * (u[0] * u[2] - s * u[1]);
		data[10] = 2 * (u[1] * u[2] + s * u[0]);
		data[11] = 2 * (s * s + u[2] * u[2]) - 1;

		data[15] = 1;
		Matrix result(4,4,data);
		return result;
	}
}