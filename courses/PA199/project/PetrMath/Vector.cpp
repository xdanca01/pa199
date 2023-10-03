// This is a tutorial file. Feel free to remove it.

#include "Vector.hpp"

namespace Petr_Math {
		void Vector::copy(Vector const& vec)
		{
			for (int i = 0; i < size; ++i)
				data[i] = vec.data[i];
		};
		Vector::Vector(int s) : size(s), data(new float[s]) {};
		Vector::~Vector()
		{
			delete[] data;
		}
		Vector::Vector(Vector const& vec) : size(vec.size), data(new float[vec.size]) { copy(vec); }
		Vector::Vector(float x, float y, float z, float w) : size(4), data(new float[4])
		{
			data[0] = x;
			data[1] = y;
			data[2] = z;
			data[3] = w;
		}
		Vector::Vector(int s, float num) : size(s), data(new float[s]())
		{
			for (int i = 0; i < size; ++i)
			{
				data[i] = num;
			}
		}
		Vector& Vector::operator=(Vector const& vec) { copy(vec); return *this; }
		float Vector::operator[](int const i) const { return data[i]; }
		float& Vector::operator[](int const i) { return data[i]; }
		float Vector::magnitude()
		{
			float total = 0.f;
			for (int i = 0; i < size; ++i) {
				total += data[i] * data[i];
			}
			return std::sqrt(total);
		}
		Vector Vector::opposite()
		{
			Vector result(size);
			for (int i = 0; i < size; ++i)
			{
				result[i] = data[i] * -1;
			}
			return result;
		}
			
		Vector Vector::operator+(Vector const& u)
		{
			Vector result(size);
			for (int i = 0; i < size; ++i)
			{
				result[i] = data[i] + u[i];
			}
			return result;
		}
		Vector Vector::operator-(Vector const& u)
		{
			Vector result(size);
			for (int i = 0; i < size; ++i)
			{
				result[i] = data[i] - u[i];
			}
			return result;
		}
		Vector Vector::operator*(Vector const& u)
		{
			Vector result(size);
			for (int i = 0; i < size; ++i)
			{
				result[i] = data[i] * u[i];
			}
			return result;
		}
		Vector Vector::operator/(float num)
		{
			Vector result(size);
			for (int i = 0; i < size; ++i)
			{
				result[i] = data[i]/num;
			}
			return result;
		}
		float Vector::dot(Vector const& vec)
		{
			float result = 0;
			for (int i = 0; i < size; ++i)
			{
				result += data[i] * vec[i];
			}
			return result;
		}
		Vector Vector::cross(Vector const& vec)
		{
			Vector result(size);
			result[0] = data[1] * vec[2] - data[2] * vec[1];
			result[1] = -(data[0] * vec[2] - data[2] * vec[0]);
			result[2] = data[0] * vec[1] - data[1] * vec[0];
			result[3] = data[3];
			return result;
		}
}