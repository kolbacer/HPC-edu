#pragma once

#include <iostream>

class Matrix
{
public:
    size_t getRows();
    size_t getCols();

    Matrix();

    // zero matrix
    Matrix(size_t rows, size_t cols);

    // matrix from 2d pointer
    Matrix(double** data, size_t rows, size_t cols);

    // matrix from 2d array
    template <size_t rows, size_t cols>
    Matrix(double (&data)[rows][cols])
    {
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
    Matrix(const Matrix& m);

    ~Matrix();

    Matrix& operator=(const Matrix& m);
    double* operator[](size_t i);

    Matrix copy();

    void print();

    // math
    Matrix add(Matrix m, bool inplace = false);
    Matrix multiply(Matrix m, bool inplace = false);
    Matrix multiply_scalar(double x, bool inplace = false);
    Matrix power(unsigned int k, bool inplace = false);
    double trace();
    double atrace();
    // static math
    static Matrix add(Matrix m1, Matrix m2);
    static Matrix multiply(Matrix m1, Matrix m2);
    static Matrix multiply_scalar(Matrix m, double x);
    static double trace(Matrix m);
    static double atrace(Matrix m);

    // math_parallel
    Matrix add_parallel(Matrix m, bool inplace = false);
    Matrix multiply_parallel(Matrix m, bool inplace = false);
    Matrix multiply_scalar_parallel(double x, bool inplace = false);
    Matrix power_parallel(unsigned int k, bool inplace = false);
    // static math_parallel
    static Matrix add_parallel(Matrix m1, Matrix m2);
    static Matrix multiply_parallel(Matrix m1, Matrix m2);
    static Matrix multiply_scalar_parallel(Matrix m, double x);

    // utils
    static Matrix create_square(size_t n);
    static Matrix create_identity(size_t n);
    static Matrix create_random(size_t rows, size_t cols, unsigned int seed);
    static Matrix create_random(size_t rows, size_t cols);
    bool equal(Matrix m, double eps = 1E-10);
    static bool equal(Matrix m1, Matrix m2, double eps = 1E-10);


private:
    size_t rows_, cols_;
    double** data_;

    void dataAlloc();
    void dataDelete();
    void reassign(double** data, size_t rows, size_t cols);
    double** copyData();
    static double** getNewData(size_t rows, size_t cols);
};