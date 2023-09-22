// This is a tutorial file. Feel free to remove it.

#pragma once
#include "Vector.hpp"

namespace Petr_Math {

	class Matrix {
	public:
		float* data;
		int width;
		int height;
		void copy(Matrix const& mat);
		explicit Matrix(int w, int h);
		~Matrix();
		Matrix(Matrix const& mat);
		Matrix(int w, int h, float num);
		Vector& operator=(Vector const& vec);
		float at(int const w, int const h);
		Matrix operator*(Matrix const& mat);
		Matrix transpose();
		Matrix operator*(Vector const& vec);
	};
}