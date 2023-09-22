// This is a tutorial file. Feel free to remove it.
#include <cmath>

#pragma once

namespace Petr_Math {

	class Vector {
	public:
		float* data;
		int size;
		void copy(Vector const& vec);
		Vector(int s);
		~Vector();
		Vector(Vector const& vec);
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
		float dot(Vector const& vec);
		Vector cross(Vector const& vec);
	};
}