// This is a tutorial file. Feel free to remove it.

#pragma once
#include "Vector.hpp"
#include <vector>

namespace Petr_Math {

	class Matrix {
	private:
		std::vector<float> data;
	public:
		struct __access_helper__ {
			explicit __access_helper__(float* row_):row{ row_ } {}
			float operator[](int shift) const
			{
				return *(row + shift);
			}
		private:
			float* row;
		};
		int width;
		int height;
		void copy(Matrix const& mat);
		explicit Matrix(int w, int h);
		~Matrix();
		Matrix(Matrix const& mat);
		/*Matrix(Matrix&& mat);*/
		Matrix& operator =(Matrix const& mat);
		Matrix(int w, int h, float num);
		Matrix(int w, int h, float* nums);
		//w*h identity matrix
		Matrix(int wh, float num, bool identity);
		void translate(float x, float y, float z);
		void translate(Vector moveVec);
		Vector& operator=(Vector const& vec);
		float at(int const w, int const h) const;
		float& at(int const w, int const h);
		Matrix operator*(Matrix const& mat);
		Matrix transpose();
		Matrix operator*(Vector const& vec);
		//[row][column]
		__access_helper__ operator[](int row_index) const;
		float* getData();
	};
}