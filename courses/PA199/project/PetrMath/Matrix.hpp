// This is a tutorial file. Feel free to remove it.

#pragma once
#include "Vector.hpp"
#include <vector>

#define M_PI       3.14159265358979323846   // pi
#define R2D 180.0f/M_PI
#define D2R M_PI/180.0f

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
		void rotateY(float angle);
		Vector& operator=(Vector const& vec);
		float at(int const w, int const h) const;
		float& at(int const w, int const h);
		Matrix operator*(Matrix const& mat);
		Matrix transpose();
		Matrix negative();
		Matrix operator*(Vector const& vec);
		//[row][column]
		__access_helper__ operator[](int row_index) const;
		float* getData();
	};
}