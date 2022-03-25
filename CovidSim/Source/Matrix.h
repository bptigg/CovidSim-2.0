#pragma once

#include <vector>
#include <iostream>

template<typename T>
class Matrix
{
private:
	unsigned int m_rowsize;
	unsigned int m_colsize;

	std::vector<std::vector<T>> matrix;

public:

	Matrix(unsigned int row_num, unsigned int col_num, T value = 0)
		:m_rowsize(row_num), m_colsize(col_num)
	{
		matrix.resize(row_num);
		for (int i = 0; i < matrix.size(); i++)
		{
			matrix[i].resize(col_num, value);
		}
	}

	Matrix(const Matrix<T>& rhs)
	{
		matrix = rhs.matrix;
		m_rowsize = rhs.get_row_size();
		m_colsize = rhs.get_col_size();
	}

	Matrix<T> operator=(const Matrix<T>& rhs)
	{
		if (&rhs == this)
		{
			return *this;
		}

		unsigned int new_rows = rhs.get_row_size();
		unsigned int new_cols = rhs.get_col_size();

		matrix.resize(new_rows);
		for (unsigned int i = 0; i < matrix.size(); i++)
		{
			matrix[i].resize(new_cols);
		}

		for (unsigned int i = 0; i < new_rows; i++)
		{
			for (unsigned int j = 0; j < new_cols; j++)
			{
				matrix[i][j] = rhs(i, j);
			}
		}

		m_rowsize = new_rows;
		m_colsize = new_cols;

		return *this;
	}
	
	Matrix<T> operator+(const Matrix<T>& rhs)
	{
		Matrix return_matrix(m_rowsize, m_colsize);

		for (unsigned int i = 0; i < m_rowsize; i++)
		{
			for (unsigned int j = 0; j < m_colsize; j++)
			{
				return_matrix(i, j) = this->matrix[i][j] + rhs(i, j);
			}
		}

		return return_matrix;
	}
	
	Matrix<T> operator-(const Matrix<T>& rhs)
	{
		Matrix return_matrix(m_rowsize, m_colsize);

		for (unsigned int i = 0; i < m_rowsize; i++)
		{
			for (unsigned int j = 0; j < m_colsize; j++)
			{
				return_matrix(i, j) = this->matrix[i][j] - rhs(i, j);
			}
		}

		return return_matrix;
	}
	
	Matrix<T> operator*(const Matrix<T>& rhs)
	{
		unsigned int rows = rhs.get_row_size();
		unsigned int cols = rhs.get_col_size();
		Matrix return_matrix(rows, cols);

		for (unsigned int i = 0; i < rows; i++)
		{
			for (unsigned int j = 0; j < cols; j++)
			{
				for (unsigned int k = 0; k < rows; k++)
				{
					return_matrix(i, j) = return_matrix(i, j) + this->matrix[i][k] * rhs(k, j);
				}
			}
		}

		return return_matrix;
	}
	
	Matrix<T> transpose()
	{
		Matrix return_matrix(m_rowsize, m_colsize);
		for (unsigned int i = 0; i < m_rowsize; i++)
		{
			for (unsigned int j = 0; j < m_colsize; j++)
			{
				return_matrix(i, j) = this->matrix[j][i];
			}
		}

		return return_matrix;

	}

	Matrix<T> operator+(const T& rhs)
	{
		Matrix return_matrix(m_rowsize, m_colsize);

		for (unsigned int i = 0; i < m_rowsize; i++)
		{
			for (unsigned int j = 0; j < m_colsize; j++)
			{
				return_matrix(i, j) = this->matrix[i][j] + rhs;
			}
		}

		return return_matrix;
	}
	
	Matrix<T> operator-(const T& rhs)
	{
		Matrix return_matrix(m_rowsize, m_colsize);

		for (unsigned int i = 0; i < m_rowsize; i++)
		{
			for (unsigned int j = 0; j < m_colsize; j++)
			{
				return_matrix(i, j) = this->matrix[i][j] - rhs;
			}
		}

		return return_matrix;
	}
	
	Matrix<T> operator*(const T& rhs)
	{
		Matrix return_matrix(m_rowsize, m_colsize);

		for (unsigned int i = 0; i < m_rowsize; i++)
		{
			for (unsigned int j = 0; j < m_colsize; j++)
			{
				return_matrix(i, j) = this->matrix[i][j] * rhs;
			}
		}

		return return_matrix;
	}
	
	Matrix<T> operator/(const T& rhs)
	{
		Matrix return_matrix(m_rowsize, m_colsize);

		for (unsigned int i = 0; i < m_rowsize; i++)
		{
			for (unsigned int j = 0; j < m_colsize; j++)
			{
				return_matrix(i, j) = this->matrix[i][j] / rhs;
			}
		}

		return return_matrix;
	}

	friend std::ostream& operator<<(std::ostream& stream, const Matrix<T>& mat)
	{
		for (int i = 0; i < mat.get_row_size(); i++)
		{
			for (int j = 0; j < mat.get_col_size(); j++)
			{
				stream << mat(i, j) << " ";
			}
			stream << "\n";
		}
		return stream;
	}

	
	T& operator()(const unsigned int& row, const unsigned int& col)
	{
		return this->matrix[row][col];
	}

	const T& operator()(const unsigned int& row, const unsigned int& col) const
	{
		return this->matrix[row][col];
	}

	
	std::vector<T>& operator()(const unsigned int& row)
	{
		return this->matrix[row];
	}
	
	const std::vector<T>& operator()(const unsigned int& row) const
	{
		return this->matrix[row];
	}


	unsigned int get_row_size() const
	{
		return m_rowsize;
	}
	
	unsigned int get_col_size() const
	{
		return m_colsize;
	}
};


