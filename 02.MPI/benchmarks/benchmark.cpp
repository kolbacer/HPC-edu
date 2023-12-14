#include <mpi.h>
#include <stdio.h>
#include <chrono>
#include "matrix.h"

void setup();
void problem();
double measure_time(void (*f)(), unsigned int iters = 1);

int size, rank;
int k;    // number of iterations
size_t n; // matrix size

Matrix B, C, E, I;

int main(int argc, char** argv)
{
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    k = std::stoi(argv[1]);
    n = std::stoi(argv[2]);

    setup();

    double res = measure_time(problem, k);
    if (rank == 0)
        printf("%.3f", res);

    MPI_Finalize();
}


void setup() {
    if (rank == 0) {
        B = Matrix::create_random(n, n, 333);
        C = Matrix::create_random(n, n, 444);
        E = Matrix::create_of(n, n, 1);
        I = Matrix::create_identity(n);
    }
}

// A = Tr(B*C^3)*E + Tr'(B)*I
void problem() {
    Matrix s1, s2;

    s1 = E.multiply_scalar_parallel(Matrix::trace(Matrix::multiply_parallel(B, C.power_parallel(3))));
    s2 = I.multiply_scalar_parallel(Matrix::atrace(B));
    s1.add_parallel(s2);
}

double measure_time(void (*f)(), unsigned int iters) {
    using std::chrono::high_resolution_clock;
    using std::chrono::duration;

    double times[iters];

    for (int i = 0; i < iters; ++i) {
        MPI_Barrier(MPI_COMM_WORLD);
        auto t1 = high_resolution_clock::now();
        (*f)();
        MPI_Barrier(MPI_COMM_WORLD);
        auto t2 = high_resolution_clock::now();
        duration<double, std::milli> ms_double = t2 - t1;
        times[i] = ms_double.count();
    }

    double average = 0;
    for (int i = 0; i < iters; ++i) {
        average += times[i];
    }
    average = average / iters;

    return average;
}