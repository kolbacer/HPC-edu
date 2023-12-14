#include <mpi.h>
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
bool test_11_problem_parallel();

bool exec_test(bool (*f)(), bool parallel = false);

int size, rank;

int main(int argc, char** argv)
{
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int testcase = std::stoi(argv[1]);
    if (rank == 0)
        printf("testcase: %d \n", testcase);

    switch(testcase)
    {
        case 1:
            return exec_test(test_1_add);
        case 2:
            return exec_test(test_2_multiply);
        case 3:
            return exec_test(test_3_trace);
        case 4:
            return exec_test(test_4_atrace);
        case 5:
            return exec_test(test_5_power);
        case 6:
            return exec_test(test_6_multiply_scalar);
        case 7:
            return exec_test(test_7_add_parallel, true);
        case 8:
            return exec_test(test_8_multiply_parallel, true);
        case 9:
            return exec_test(test_9_multiply_scalar_parallel, true);
        case 10:
            return exec_test(test_10_power_parallel, true);
        case 11:
            return exec_test(test_11_problem_parallel, true);
        default:
            return 2;
    }
}

bool exec_test(bool (*f)(), bool parallel) {
    bool result = f();
    MPI_Finalize();
    return result;
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
    Matrix matrix1, matrix2, add_seq, add_prl;

    if (rank == 0) {
        matrix1 = Matrix::create_random(100, 100, 777);
        matrix2 = Matrix::create_random(100, 100, 555);
        add_seq = matrix1.add(matrix2);
    }

    add_prl = matrix1.add_parallel(matrix2);

    if (rank != 0) return 0;
    return Matrix::equal(add_seq, add_prl) ? 0 : 1;
}

bool test_8_multiply_parallel() {
    Matrix matrix1, matrix2, mul_seq, mul_prl;

    if (rank == 0) {
        matrix1 = Matrix::create_random(100, 100, 777);
        matrix2 = Matrix::create_random(100, 100, 555);
        mul_seq = matrix1.multiply(matrix2);
    }

    mul_prl = matrix1.multiply_parallel(matrix2);

    if (rank != 0) return 0;
    return Matrix::equal(mul_seq, mul_prl) ? 0 : 1;
}

bool test_9_multiply_scalar_parallel() {
    Matrix matrix, mul_scal_seq, mul_scal_prl;
    double scalar = 5.721;

    if (rank == 0) {
        matrix = Matrix::create_random(100, 100, 777);
        mul_scal_seq = matrix.multiply_scalar(scalar);
    }

    mul_scal_prl = matrix.multiply_scalar_parallel(scalar);

    if (rank != 0) return 0;
    return Matrix::equal(mul_scal_seq, mul_scal_prl) ? 0 : 1;
}

bool test_10_power_parallel() {
    Matrix matrix, pow_seq, pow_prl;
    unsigned int pow = 3;

    if (rank == 0) {
        matrix = Matrix::create_random(100, 100, 777);
        pow_seq = matrix.power(pow);
    }

    pow_prl = matrix.power_parallel(pow);

    if (rank != 0) return 0;
    return Matrix::equal(pow_seq, pow_prl) ? 0 : 1;
}

// A = Tr(B*C^3)*E + Tr'(B)*I
bool test_11_problem_parallel() {
    Matrix B, C, E, I, s1, s2, A_seq, A_prl;
    size_t n = 100;

    if (rank == 0) {
        B = Matrix::create_random(n, n, 333);
        C = Matrix::create_random(n, n, 444);
        E = Matrix::create_of(n, n, 1);
        I = Matrix::create_identity(n);
    }

    // sequential
    if (rank == 0) {
        s1 = E.multiply_scalar(Matrix::trace(Matrix::multiply(B, C.power(3))));
        s2 = I.multiply_scalar(Matrix::atrace(B));
        A_seq = s1.add(s2);
    }

    // parallel
    s1 = E.multiply_scalar_parallel(Matrix::trace(Matrix::multiply_parallel(B, C.power_parallel(3))));
    s2 = I.multiply_scalar_parallel(Matrix::atrace(B));
    A_prl = s1.add_parallel(s2);

    if (rank != 0) return 0;
    return Matrix::equal(A_seq, A_prl) ? 0 : 1;
}