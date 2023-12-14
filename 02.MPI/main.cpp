#include <stdio.h>
#include <mpi.h>
#include "matrix.h"

int main(int argc, char** argv)
{
    int size, rank;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) {
        printf("### Example ###\n\n");
        printf("check processors:\n");
    }

    MPI_Barrier(MPI_COMM_WORLD);

    char processor_name[MPI_MAX_PROCESSOR_NAME];
    int name_len;
    MPI_Get_processor_name(processor_name, &name_len);

    printf("Hello from processor %s, rank %d out of %d processors\n", processor_name, rank, size);

    MPI_Barrier(MPI_COMM_WORLD);

    if (rank == 0)
        printf("\n");


    Matrix m1, m2;

    if (rank == 0) {
        double data1[3][3] = {{1.356, 2.456, 5.45675432},
                              {0.345, 4.3, 5},
                              {9.3545425, 5.43, 6}};
        m1 = Matrix(data1);
        printf("matrix1\n");
        m1.print();
        printf("\n");
        double data2[3][3] = {{2.45, 7.93, 3.0},
                              {1, 1, 1},
                              {0, -5, -1.1}};
        m2 = Matrix(data2);
        printf("matrix2\n");
        m2.print();
        printf("\n");
    }

    Matrix m3;
    if (rank == 0) {
        m3 = m1.add(m2);
        printf("matrix1 + matrix2 (sequential): \n");
        m3.print();
        printf("\n");
    }

    m3 = m1.add_parallel(m2);
    if (rank == 0) {
        printf("matrix1 + matrix2 (parallel): \n");
        m3.print();
        printf("\n");
    }

    Matrix m4;
    if (rank == 0) {
        m4 = m1.multiply(m2);
        printf("matrix1 * matrix2 (sequential): \n");
        m4.print();
        printf("\n");
    }

    m4 = m1.multiply_parallel(m2);
    if (rank == 0) {
        printf("matrix1 * matrix2 (parallel): \n");
        m4.print();
        printf("\n");
    }

    Matrix m5;
    if (rank == 0) {
        m5 = m1.multiply_scalar(3);
        printf("matrix1 * 3 (sequential): \n");
        m5.print();
        printf("\n");
    }

    m5 = m1.multiply_scalar_parallel(3);
    if (rank == 0) {
        printf("matrix1 * 3 (parallel): \n");
        m5.print();
        printf("\n");
    }

    Matrix m6;
    if (rank == 0) {
        m6 = m1.power(3);
        printf("matrix1 ^ 3 (sequential): \n");
        m6.print();
        printf("\n");
    }

    m6 = m1.power_parallel(3);
    if (rank == 0) {
        printf("matrix1 ^ 3 (parallel): \n");
        m6.print();
        printf("\n");
    }

    MPI_Finalize();
}