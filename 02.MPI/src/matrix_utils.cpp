#include "matrix.h"
#include <cstdlib>
#include <chrono>

Matrix Matrix::create_square(size_t n) {
    return {n ,n};
}

Matrix Matrix::create_identity(size_t n) {
    Matrix matrix = create_square(n);
    for (size_t i = 0; i < n; ++i) {
        matrix[i][i] = 1;
    }

    return matrix;
}

Matrix Matrix::create_of(size_t rows, size_t cols, double x) {
    Matrix matrix(rows, cols);
    for (size_t i = 0; i < rows; ++i)
        for (size_t j = 0; j < cols; ++j)
            matrix[i][j] = x;

    return matrix;
}

Matrix Matrix::create_random(size_t rows, size_t cols, unsigned int seed) {
    Matrix matrix = Matrix(rows, cols);

    double fMax = 10, fMin = -10;
    srand(seed);

    for (size_t i = 0; i < matrix.rows_; ++i) {
        for (size_t j = 0; j < matrix.cols_; ++j) {
            double f = (double) rand() / RAND_MAX;
            matrix[i][j] = fMin + f * (fMax - fMin);
        }
    }

    return matrix;
}

Matrix Matrix::create_random(size_t rows, size_t cols) {
    return create_random(rows, cols, std::chrono::system_clock::now().time_since_epoch().count());
}

// compare matrices with precision = eps
bool Matrix::equal(Matrix m, double eps) {
    if (rows_ != m.rows_ || cols_ != m.cols_)
        return false;

    double** m1_data = this->data_;
    double** m2_data = m.data_;

    for (size_t i = 0; i < rows_; ++i) {
        for (size_t j = 0; j < cols_; ++j) {
            if (std::abs(m1_data[i][j] - m2_data[i][j]) > eps)
                return false;
        }
    }

    return true;
}

// static equal()
bool Matrix::equal(Matrix m1, Matrix m2, double eps) {
    return m1.equal(m2, eps);
}