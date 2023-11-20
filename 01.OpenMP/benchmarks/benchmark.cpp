#include <stdio.h>
#include <chrono>
#include <omp.h>
#include "matrix.h"

void setup();
void problem();
double experiment(int k_matrix, int k_threads);
double measure_time(void (*f)(), unsigned int iters = 1);

const int k_matrix_cases = 3;
const int k_threads_cases = 12;
const int k_iters = 3;
Matrix B, C, E;
Matrix B_list[k_matrix_cases], C_list[k_matrix_cases], E_list[k_matrix_cases];
size_t sizes[k_matrix_cases] = {100, 1000, 2000};
size_t num_threads_list[k_threads_cases] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};

double results[k_matrix_cases][k_threads_cases];


int main()
{
    setup();

    printf("bechmarks start (iters=%d) \n", k_iters);
    for (int i = 0; i < k_matrix_cases; ++i) {
        printf("  matrix case %d (%dx%d)\n", i, sizes[i], sizes[i]);
        for (int j = 0; j < k_threads_cases; ++j) {
            printf("    run with threads=%d ... ", num_threads_list[j]);
            double res = experiment(i, j);
            printf("result: %f ms\n", res);
            results[i][j] = res;
        }
    }
    printf("bechmarks end\n\n");

    printf("Summary:\n");
    printf("Matrix \\ Threads ");
    for (int j = 0; j < k_threads_cases; ++j) {
        printf("%5d ", num_threads_list[j]);
    }
    printf("\n");
    for (int i = 0; i < k_matrix_cases; ++i) {
        printf("%5d  x%5d:    ", sizes[i], sizes[i]);
        for (int j = 0; j < k_threads_cases; ++j) {
            printf("%3.2f  ", results[i][j]);
        }
        printf("\n");
    }
    printf("\n\n");

    printf("Raw:\n");
    for (int i = 0; i < k_matrix_cases; ++i) {
        for (int j = 0; j < k_threads_cases; ++j) {
            printf("%.3f ", results[i][j]);
        }
        printf("\n");
    }
}


void setup() {
    for (int i = 0; i < k_matrix_cases; ++i) {
        B_list[i] = Matrix::create_random(sizes[i], sizes[i], 555+i);
        C_list[i] = Matrix::create_random(sizes[i], sizes[i], 777+i);
        E_list[i] = Matrix::create_identity(sizes[i]);
    }
}

// A = Tr(B*C^3)*E + Tr'(B)*E
void problem() {
    omp_set_nested(1);

    Matrix s1, s2;
    #pragma omp parallel
    #pragma omp single nowait
    {
        #pragma omp task shared(E,B,C,s1) default(none)
        s1 = E.multiply_scalar_parallel(Matrix::trace(Matrix::multiply_parallel(B, C.power_parallel(3))));

        #pragma omp task shared(E,B,s2) default(none)
        s2 = E.multiply_scalar_parallel(Matrix::atrace(B));
    }
    #pragma omp taskwait

    s1.add_parallel(s2);
}

double experiment(int k_matrix, int k_threads) {
    B = B_list[k_matrix];
    C = C_list[k_matrix];
    E = E_list[k_matrix];
    omp_set_num_threads(num_threads_list[k_threads]);
    return measure_time(problem, k_iters);
}

double measure_time(void (*f)(), unsigned int iters) {
    using std::chrono::high_resolution_clock;
    using std::chrono::duration;

    double times[iters];

    for (int i = 0; i < iters; ++i) {
        auto t1 = high_resolution_clock::now();
        (*f)();
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