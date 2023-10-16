// This is a tutorial file. Feel free to remove it.
#include <cmath>
#include <vector>

#pragma once

namespace Petr_Math {

	class Vector {
	private:
		std::vector<float> data;
	public:
		int size;
		void copy(Vector const& vec);
		Vector(int s);
		~Vector();
		Vector(Vector const& vec);
		Vector(float x, float y, float z);
		Vector(float x, float y, float z, float w);
		Vector(int s, float num);
		Vector& operator=(Vector const& vec);
		float operator[](int const i) const;
		float& operator[](int const i);
		float magnitude();
		Vector opposite();
		Vector operator+(Vector const& u);
		Vector operator-(Vector const& u);
		Vector operator*(Vector const& u);
		Vector operator*(float num);
		Vector operator/(float num);
		float dot(Vector const& vec);
		Vector cross(Vector const& vec);
		Vector normalize();
		float* getData();
	};
}