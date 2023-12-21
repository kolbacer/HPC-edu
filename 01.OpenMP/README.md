# Task H (OpenMP)

Write a program to evaluate a matrix expression:

$A = Tr(BC^3)E+Tr'(B)I$,

where $B,C$ are square dense matrices, the elements of which are of the **double** type, $I$ is the identity matrix, $Tr(M)$ is the trace of the matrix $M$, $Tr'(M)$ is the "antitrace" of the matrix $M$ (the sum of the elements of the antidiagonal). Parallelize this program using OpenMP (parallel, task). Study the dependence of the scalability of a parallel version of a program on its computational complexity (matrix size). Check if the parallel version is correct.

Verification of Amdahl's Law. Build a dependency *speedup:number of threads* for a given example.

## Prerequisites
- `CMake 3.5+`
- `some C++11 compiler`
- `OpenMP 3.0+`

## Usage
### Configure
In project root:
```console
usr:~/01.OpenMP$ mkdir build
usr:~/01.OpenMP$ cd build
usr:~/01.OpenMP/build$ cmake ..
```
### Build
```console
usr:~/01.OpenMP/build$ cmake --build .
```
### Run example
```console
usr:~/01.OpenMP/build$ ./HPC_01_OpenMP
```
### Run tests
```console
usr:~/01.OpenMP/build$ ctest
```
### Run benchmarks
```console
usr:~/01.OpenMP/build$ ./HPC_01_benchmark
```

## Performance

Software & hardware configuration:

| | |
| --- | --- |
OS | Ubuntu 22.04.2 LTS |
Cores | 6 |
Threads |	12 |
Processor model |	Intel(R) Xeon(R) E-2136 CPU @ 3.30GHz |
RAM | 64GiB |
Compiler | g++ (GCC) 10.2.0 |

Dependence of execution time on the number of threads ($n$ - matrix size):

![OpenMP_time](results/OpenMP_time.png)

Dependence of speedup on the number of threads: 

![OpenMP_speedup](results/OpenMP_speedup.png)

Speedup $S(n)=T(1)/T(n)$, where $T(n)$ - execution time of the algorithm on $n$ processors.


## Project structure
```
.
├── README.md - project description (english)
├── README.ru.md - project description (russian)
├── CMakeLists.txt - CMake build script
├── results/ - folder with experiment results
├── main.cpp - entry point (examples)
├── include - headers
│   └── matrix.h - matrix class description
├── src
│   ├── matrix_base.cpp - essential methods
│   ├── matrix_math.cpp - matrix algebra
│   ├── matrix_math_parallel.cpp - parallel methods
│   └── matrix_utils.cpp - helper methods
├── tests - unit tests
│   └── test.cpp
└── benchmarks - benchmarks for the task
    └── benchmark.cpp
```
