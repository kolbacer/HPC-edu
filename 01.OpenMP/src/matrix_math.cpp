#include "matrix.h"

Matrix Matrix::add(Matrix m, bool inplace) {
    if (rows_ != m.rows_ || cols_ != m.cols_)
        throw std::invalid_argument("incompatible matrix dimensions");

    Matrix result = inplace ? *this : Matrix(rows_, cols_);
    double** data = result.data_;
    double** m1_data = this->data_;
    double** m2_data = m.data_;

    for (size_t i = 0; i < rows_; ++i) {
        for (size_t j = 0; j < cols_; ++j) {
            data[i][j] = m1_data[i][j] + m2_data[i][j];
        }
    }

    return result;
}

Matrix Matrix::multiply(Matrix m, bool inplace) {
    if (cols_ != m.rows_)
        throw std::invalid_argument("incompatible matrix dimensions");

    size_t newRows = rows_, newCols = m.cols_;
    double** data = Matrix::getNewData(newRows, newCols);
    Matrix &m1 = *this;
    Matrix &m2 = m;

    double** m1_data = m1.data_;
    double** m2_data = m2.data_;

    for (size_t i = 0; i < newRows; ++i) {
        for (size_t j = 0; j < newCols; ++j) {
            double x = 0;
            for (size_t k = 0; k < m1.cols_; ++k) {
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

Matrix Matrix::multiply_scalar(double x, bool inplace) {
    double** data = Matrix::getNewData(rows_, cols_);

    for (size_t i = 0; i < rows_; ++i) {
        for (size_t j = 0; j < cols_; ++j) {
            data[i][j] = data_[i][j]*x;
        }
    }

    if (inplace) {
        reassign(data, rows_, cols_);
        return *this;
    }

    return Matrix(data, rows_, cols_);
}

Matrix Matrix::power(unsigned int k, bool inplace) {
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
        result.multiply(*this, true);
    }

    if (inplace) {
        reassign(result.copyData(), n, n);
        return *this;
    }

    return Matrix(result.copyData(), n, n);
}

// sum of diagonal elements
double Matrix::trace() {
    if (cols_ != rows_)
        throw std::logic_error("matrix is not square");

    double tr = 0;
    for (size_t i = 0; i < rows_; ++i) {
        tr += data_[i][i];
    }

    return tr;
}

// sum of antidiagonal elements
double Matrix::atrace() {
    if (cols_ != rows_)
        throw std::logic_error("matrix is not square");

    size_t n = rows_;
    double atr = 0;
    for (size_t i = 0; i < n; ++i) {
        atr += data_[i][n-1-i];
    }

    return atr;
}

// static methods

Matrix Matrix::add(Matrix m1, Matrix m2) {
    return m1.add(m2);
}

Matrix Matrix::multiply(Matrix m1, Matrix m2) {
    return m1.multiply(m2);
}

Matrix Matrix::multiply_scalar(Matrix m, double x) {
    return m.multiply_scalar(x);
}

double Matrix::trace(Matrix m) {
    return m.trace();
}

double Matrix::atrace(Matrix m) {
    return m.atrace();
}