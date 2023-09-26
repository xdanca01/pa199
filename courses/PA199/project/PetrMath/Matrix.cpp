// This is a tutorial file. Feel free to remove it.

#include "Matrix.hpp"

namespace Petr_Math {

	void Matrix::copy(Matrix const& mat)
	{
		for (int i = 0; i < height * width; ++i)
		{
			data[i] = mat.data[i];
		}
	}

	Matrix::Matrix(int const h, int const w) : width(w), height(h), data(new float[w*h]){}

	Matrix::~Matrix()
	{
		delete data;
	}

	Matrix::Matrix(Matrix const& mat) : width(mat.width), height(mat.height), data(new float[width * height])
	{
		for (int i = 0; i < height * width; ++i)
		{
			data[i] = mat.data[i];
		}
	}

	Matrix::Matrix(int w, int h, float num) : width(w), height(h), data(new float[w * h])
	{
		for (int i = 0; i < height * width; ++i)
		{
			data[i] = num;
		}
	}

	Matrix::Matrix(int w, int h, float* nums) : width(w), height(h), data(new float[w * h])
	{
		for (int i = 0; i < height * width; ++i)
		{
			data[i] = nums[i];
		}
	}

	float Matrix::at(int const h, int const w) const
	{
		return data[w + h * width];
	};

	float& Matrix::at(int const w, int const h)
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
				result.data[j * height + i] = this->at(i, j);
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

	Petr_Math::Matrix::__access_helper__ Matrix::operator[](int row_index) const
	{
		return __access_helper__{ data + row_index * width };
	}

}