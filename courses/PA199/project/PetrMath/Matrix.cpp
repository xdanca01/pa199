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

	explicit Matrix::Matrix(int const h, int const w) : width(w), height(h), data(new float[w*h]){}

	float Matrix::at(int const h, int const w)
	{
		return data[w + h * width];
	};

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
}