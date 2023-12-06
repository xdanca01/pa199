// This is a tutorial file. Feel free to remove it.

#pragma once

#include "Matrix.hpp"

namespace Petr_Math {

	void Matrix::copy(Matrix const& mat)
	{
		for (int i = 0; i < height * width; ++i)
		{
			data[i] = mat.data[i];
		}
	}

	Matrix::Matrix(int const h, int const w) : width(w), height(h){
		data.resize(w * h);
	}

	Matrix::~Matrix()
	{
	}

	Matrix::Matrix(Matrix const& mat) : width(mat.width), height(mat.height)
	{
		data.resize(width * height);
		for (int i = 0; i < height * width; ++i)
		{
			data[i] = mat.data[i];
		}
	}

	Matrix::Matrix(int h, int w, float num) : width(w), height(h)
	{
		data.resize(width * height);
		for (int i = 0; i < height * width; ++i)
		{
			data[i] = num;
		}
	}

	Matrix::Matrix(int wh, float num, bool identity) : width(wh), height(wh)
	{
		data.resize(wh * wh);
		for (int i = 0; i < wh; ++i)
		{
			for (int j = 0; j < wh; ++j)
			{
				if (i == j)
				{
					data[i * wh + j] = num;
				}
				else
				{
					data[i * wh + j] = 0.0f;
				}
			}
		}
	}
	void Matrix::translate(Vector moveVec)
	{
		at(0, 3) += moveVec[0];
		at(1, 3) += moveVec[1];
		at(2, 3) += moveVec[2];
	}


	void Matrix::translate(float x, float y, float z)
	{
		at(0, 3) += x;
		at(1, 3) += y;
		at(2, 3) += z;
	}

	void Matrix::rotateY(float angle)
	{
		angle = -angle * D2R;
		at(0, 0) = cos(angle);
		at(0, 2) = sin(angle);
		at(2, 0) = -sin(angle);
		at(2, 2) = cos(angle);
	}

	Matrix& Matrix::operator =(Matrix const& mat)
	{
		width = mat.width;
		height = mat.height;
		data.resize(width * height);
		for (int i = 0; i < height * width; ++i)
		{
			data[i] = mat.data[i];
		}
		return *this;
	}

	Matrix::Matrix(int h, int w, float* nums) : width(w), height(h)
	{
		data.resize(w * h);
		for (int i = 0; i < height * width; ++i)
		{
			data[i] = nums[i];
		}
	}

	//[row, column]
	float Matrix::at(int const h, int const w) const
	{
		return data[w + h * width];
	};

	//[row, column]
	float& Matrix::at(int const h, int const w)
	{
		return data[w + h * width];
	}

	Matrix Matrix::operator*(Matrix const& mat)
	{
		Matrix result(height, mat.width);
		for (int i = 0; i < height; ++i)
		{
			for (int j = 0; j < mat.width; ++j)
			{
				result.data[i * result.width + j] = 0;
				for (int v = 0; v < width; ++v)
				{
					result.data[i * result.width + j] += data[i * width + v] * mat.data[v * mat.height + j];
				}
			}
		}
		return result;
	}

	Matrix Matrix::transpose()
	{
		Matrix result(width, height);
		for (int i = 0; i < height; ++i)
		{
			for (int j = 0; j < width; ++j)
			{
				result.at(j, i) = this->at(i, j);
			}
		}
		return result;
	}

	Matrix Matrix::negative()
	{
		Matrix result(width, height);
		for (int i = 0; i < height; ++i)
		{
			for (int j = 0; j < width; ++j)
			{
				result.at(i, j) = -this->at(i, j);
			}
		}
		return result;
	}

	Matrix Matrix::operator*(Vector const& vec)
	{
		Matrix result(height, 1);
		for (int i = 0; i < height; ++i)
		{
			result.data[i * result.width] = 0;
			for (int v = 0; v < vec.size; ++v)
			{
				result.data[i * result.width] += data[i * width + v] * vec[v];
			}
		}
		return result;
	}

	float* Matrix::getData()
	{
		return data.data();
	}
}