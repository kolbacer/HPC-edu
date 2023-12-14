#include "matrix.h"
#include <stdio.h>

size_t Matrix::getRows() { return rows_; }
size_t Matrix::getCols() { return cols_; }

Matrix::Matrix() {
    rows_ = 0;
    cols_ = 0;
    data_ = nullptr;
}

// zero matrix
Matrix::Matrix(size_t rows, size_t cols) {
    rows_ = rows;
    cols_ = cols;
    dataAlloc();
    for (size_t i = 0; i < rows_; ++i) {
        for (size_t j = 0; j < cols_; ++j) {
            data_[i][j] = 0;
        }
    }
}

// matrix from 2d pointer
Matrix::Matrix(double** data, size_t rows, size_t cols) {
    rows_ = rows;
    cols_ = cols;
    dataAlloc();
    for (size_t i = 0; i < rows_; ++i) {
        for (size_t j = 0; j < cols_; ++j) {
            data_[i][j] = data[i][j];
        }
    }
}

// matrix from other matrix
Matrix::Matrix(const Matrix& m) {
    rows_ = m.rows_;
    cols_ = m.cols_;
    dataAlloc();
    for (size_t i = 0; i < rows_; ++i) {
        for (size_t j = 0; j < cols_; ++j) {
            data_[i][j] = m.data_[i][j];
        }
    }
}

Matrix::~Matrix() {
    dataDelete();
}

Matrix& Matrix::operator=(const Matrix& m) {
    if (this == &m) {
        return *this;
    }

    if (rows_ != m.rows_ || cols_ != m.cols_) {
        dataDelete();
        rows_ = m.rows_;
        cols_ = m.cols_;
        dataAlloc();
    }

    for (int i = 0; i < rows_; ++i) {
        for (int j = 0; j < cols_; ++j) {
            data_[i][j] = m.data_[i][j];
        }
    }

    return *this;
}

double* Matrix::operator[](size_t i) {
    if (data_ == nullptr)
        return nullptr;
    return data_[i];
}

Matrix Matrix::copy() {
    Matrix newMatrix = Matrix(rows_, cols_);
    double** newData = newMatrix.data_;
    for (size_t i = 0; i < rows_; ++i) {
        for (size_t j = 0; j < cols_; ++j) {
            newData[i][j] = data_[i][j];
        }
    }
    return newMatrix;
}

void Matrix::print() {
    for (size_t i = 0; i < rows_; ++i) {
        for (size_t j = 0; j < cols_; ++j) {
            printf("%.2f ", data_[i][j]);
        }
        printf("\n");
    }
}

// contiguous data allocation
void Matrix::dataAlloc() {
    if (rows_ == 0 || cols_ == 0) {
        data_ = nullptr;
        return;
    }
    double* arr = new double[rows_ * cols_];
    data_ = new double*[rows_];
    for (size_t i = 0; i < rows_; ++i) {
        data_[i] = arr + i*cols_;
    }
}

void Matrix::dataDelete() {
    if (data_ == nullptr) return;
    delete[] data_[0];
    delete[] data_;
}

void Matrix::reassign(double** data, size_t rows, size_t cols) {
    if (this->data_ == data) return;

    dataDelete();
    this->rows_ = rows;
    this->cols_ = cols;
    this->data_ = data;
}

double** Matrix::copyData() {
    double* arr = new double[rows_ * cols_];
    double** data = new double*[rows_];
    for (size_t i = 0; i < rows_; ++i) {
        data[i] = arr + i*cols_;
    }

    for (size_t i = 0; i < rows_; ++i) {
        for (size_t j = 0; j < cols_; ++j) {
            data[i][j] = data_[i][j];
        }
    }

    return data;
}

double** Matrix::getNewData(size_t rows, size_t cols) {
    if (rows == 0 || cols == 0) return nullptr;
    double* arr = new double[rows * cols];
    double** data = new double*[rows];
    for (size_t i = 0; i < rows; ++i) {
        data[i] = arr + i*cols;
    }
    return data;
}