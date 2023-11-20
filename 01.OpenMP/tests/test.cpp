#include "matrix.h"

bool test_1_add();
bool test_2_multiply();
bool test_3_trace();
bool test_4_atrace();
bool test_5_power();
bool test_6_multiply_scalar();
bool test_7_add_parallel();
bool test_8_multiply_parallel();
bool test_9_multiply_scalar_parallel();
bool test_10_power_parallel();

int main(int argc, char* argv[])
{
    int testcase = std::stoi(argv[1]);

    switch(testcase)
    {
        case 1:
            return test_1_add();
        case 2:
            return test_2_multiply();
        case 3:
            return test_3_trace();
        case 4:
            return test_4_atrace();
        case 5:
            return test_5_power();
        case 6:
            return test_6_multiply_scalar();
        case 7:
            return test_7_add_parallel();
        case 8:
            return test_8_multiply_parallel();
        case 9:
            return test_9_multiply_scalar_parallel();
        case 10:
            return test_10_power_parallel();
        default:
            return 2;
    }
}


bool test_1_add() {
    double data1[3][3] = {{1, 2, 3},
                          {4, 5, 6},
                          {7, 8, 9}};
    double data2[3][3] = {{3, 3, 3},
                          {2, 2, 2},
                          {1, 1, 1}};
    double data3[3][3] = {{4, 5, 6},
                          {6, 7, 8},
                          {8, 9, 10}};

    Matrix matrix1 = Matrix(data1);
    Matrix matrix2 = Matrix(data2);
    Matrix matrix3 = Matrix(data3);

    return Matrix::equal(matrix1.add(matrix2), matrix3) ? 0 : 1;
}

bool test_2_multiply() {
    double data1[3][3] = {{1.356,     2.456, 5.45675432},
                          {0.345,     4.3,   5},
                          {9.3545425, 5.43,  6}};
    double data2[3][3] = {{2.45, 7.93, 3.0},
                          {1,    1,    1},
                          {0,    -5,   -1.1}};
    double data3[3][3] = {{5.7782, -14.0746916, 0.521570248},
                          {5.14525, -17.96415, -0.165},
                          {28.348629125, 49.611522025, 26.8936275}};

    Matrix matrix1 = Matrix(data1);
    Matrix matrix2 = Matrix(data2);
    Matrix matrix3 = Matrix(data3);

    return Matrix::equal(matrix1.multiply(matrix2), matrix3) ? 0 : 1;
}

bool test_3_trace() {
    double data[3][3] = {{1, 2, 3},
                         {4, 5, 6},
                         {7, 8, 9}};

    Matrix matrix = Matrix(data);
    double true_trace = 15;
    double res_trace = matrix.trace();

    return res_trace == true_trace ? 0 : 1;
}

bool test_4_atrace() {
    double data[3][3] = {{1, 2, 3},
                         {4, 5, 6},
                         {7, 8, 9}};

    Matrix matrix = Matrix(data);
    double true_atrace = 15;
    double res_atrace = matrix.atrace();

    return res_atrace == true_atrace ? 0 : 1;
}

bool test_5_power() {
    double data[3][3] = {{1, 2, 3},
                         {4, 5, 6},
                         {7, 8, 9}};

    double data_true[3][3] = {{468, 576, 684},
                              {1062, 1305, 1548},
                              {1656, 2034, 2412}};

    Matrix matrix = Matrix(data);
    Matrix true_power = Matrix(data_true);

    return Matrix::equal(matrix.power(3), true_power) ? 0 : 1;
}

bool test_6_multiply_scalar() {
    double data[3][3] = {{1, 2, 3},
                         {4, 5, 6},
                         {7, 8, 9}};

    double data_true[3][3] = {{2,  4,  6},
                              {8,  10, 12},
                              {14, 16, 18}};

    Matrix matrix = Matrix(data);
    Matrix true_mult_scalar = Matrix(data_true);

    return Matrix::equal(matrix.multiply_scalar(2), true_mult_scalar) ? 0 : 1;
}

bool test_7_add_parallel() {
    Matrix matrix1 = Matrix::create_random(100, 100, 777);
    Matrix matrix2 = Matrix::create_random(100, 100, 555);

    Matrix add_seq = matrix1.add(matrix2);
    Matrix add_prl = matrix1.add_parallel(matrix2);

    return Matrix::equal(add_seq, add_prl) ? 0 : 1;
}

bool test_8_multiply_parallel() {
    Matrix matrix1 = Matrix::create_random(100, 100, 777);
    Matrix matrix2 = Matrix::create_random(100, 100, 555);

    Matrix mul_seq = matrix1.multiply(matrix2);
    Matrix mul_prl = matrix1.multiply_parallel(matrix2);

    return Matrix::equal(mul_seq, mul_prl) ? 0 : 1;
}

bool test_9_multiply_scalar_parallel() {
    Matrix matrix = Matrix::create_random(100, 100, 777);
    double scalar = 5.721;

    Matrix mul_scal_seq = matrix.multiply_scalar(scalar);
    Matrix mul_scal_prl = matrix.multiply_scalar_parallel(scalar);

    return Matrix::equal(mul_scal_seq, mul_scal_prl) ? 0 : 1;
}

bool test_10_power_parallel() {
    Matrix matrix = Matrix::create_random(100, 100, 777);
    unsigned int pow = 3;

    Matrix pow_seq = matrix.power(pow);
    Matrix pow_prl = matrix.power_parallel(pow);

    return Matrix::equal(pow_seq, pow_prl) ? 0 : 1;
}