#include <omp.h>
#include "matrix.h"

Matrix Matrix::add_parallel(Matrix m, bool inplace) {
    if (rows_ != m.rows_ || cols_ != m.cols_)
        throw std::invalid_argument("incompatible matrix dimensions");

    Matrix result = inplace ? *this : Matrix(rows_, cols_);
    double** data = result.data_;
    double** m1_data = this->data_;
    double** m2_data = m.data_;

    size_t i,j;
    #pragma omp parallel for shared(data, m1_data, m2_data, rows_, cols_) private(i, j) default(none)
    for (i = 0; i < rows_; ++i) {
        for (j = 0; j < cols_; ++j) {
            data[i][j] = m1_data[i][j] + m2_data[i][j];
        }
    }

    return result;
}

Matrix Matrix::multiply_parallel(Matrix m, bool inplace) {
    if (cols_ != m.rows_)
        throw std::invalid_argument("incompatible matrix dimensions");

    size_t newRows = rows_, newCols = m.cols_;
    double** data = Matrix::getNewData(newRows, newCols);
    Matrix &m1 = *this;
    Matrix &m2 = m;

    double** m1_data = m1.data_;
    double** m2_data = m2.data_;

    size_t i,j,k;
    #pragma omp parallel for shared(newRows, newCols, m1, m2, data, m1_data, m2_data) private(i, j, k) default(none)
    for (i = 0; i < newRows; ++i) {
        for (j = 0; j < newCols; ++j) {
            double x = 0;
            for (k = 0; k < m1.cols_; ++k) {
                x += m1_data[i][k] * m2_data[k][j];
            }
            data[i][j] = x;
        }
    }

    if (inplace) {
        reassign(data, newRows, newCols);
        return *this;
    }

    return Matrix(data, newCols, newCols);
}

Matrix Matrix::multiply_scalar_parallel(double x, bool inplace) {
    Matrix result = inplace ? *this : Matrix(rows_, cols_);
    double** data = result.data_;

    size_t i,j;
    #pragma omp parallel for shared(x, data, rows_, cols_) private(i, j) default(none)
    for (i = 0; i < rows_; ++i) {
        for (j = 0; j < cols_; ++j) {
            data[i][j] = data_[i][j]*x;
        }
    }

    return result;
}

Matrix Matrix::power_parallel(unsigned int k, bool inplace) {
    if (cols_ != rows_)
        throw std::logic_error("matrix is not square");

    size_t n = rows_;

    if (k == 0) {
        double** data = Matrix::create_identity(n).copyData();
        if (inplace) {
            reassign(data, n, n);
            return *this;
        }
        return Matrix(data, n, n);
    }

    if (k == 1) {
        return inplace ? *this : copy();
    }

    Matrix result = copy();

    for (unsigned int i = 2; i <= k; ++i) {
        result.multiply_parallel(*this, true);
    }

    if (inplace) {
        reassign(result.copyData(), n, n);
        return *this;
    }

    return Matrix(result.copyData(), n, n);
}

// static methods

Matrix Matrix::add_parallel(Matrix m1, Matrix m2) {
    return m1.add_parallel(m2);
}

Matrix Matrix::multiply_parallel(Matrix m1, Matrix m2) {
    return m1.multiply_parallel(m2);
}

Matrix Matrix::multiply_scalar_parallel(Matrix m, double x) {
    return m.multiply_scalar_parallel(x);
}