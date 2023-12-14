cmake_minimum_required(VERSION 3.5)
project(HPC_02_MPI VERSION 0.1.0 LANGUAGES C CXX)

include(CTest)
enable_testing()

file(GLOB_RECURSE SOURCE_FILES src/*.cpp)

find_package(MPI REQUIRED)
message(STATUS "Run: ${MPIEXEC} ${MPIEXEC_NUMPROC_FLAG} ${MPIEXEC_MAX_NUMPROCS} ${MPIEXEC_PREFLAGS} EXECUTABLE ${MPIEXEC_POSTFLAGS} ARGS")
add_executable(HPC_02_MPI main.cpp ${SOURCE_FILES})

target_include_directories(HPC_02_MPI PRIVATE include)
target_link_libraries(HPC_02_MPI PRIVATE MPI::MPI_CXX)

# testing
file(GLOB_RECURSE TEST_FILES tests/*.cpp)
add_executable(HPC_02_test ${TEST_FILES} ${SOURCE_FILES})
target_include_directories(HPC_02_test PRIVATE include)
target_link_libraries(HPC_02_test PRIVATE MPI::MPI_CXX)

# non-MPI tests
add_test(test_1_add HPC_02_test 1)
add_test(test_2_multiply HPC_02_test 2)
add_test(test_3_trace HPC_02_test 3)
add_test(test_4_atrace HPC_02_test 4)
add_test(test_5_power HPC_02_test 5)
add_test(test_6_multiply_scalar HPC_02_test 6)
# MPI test
add_test(NAME test_7_add_parallel COMMAND mpiexec -n 4 $<TARGET_FILE:HPC_02_test> 7)
add_test(NAME test_8_multiply_parallel COMMAND mpiexec -n 4 $<TARGET_FILE:HPC_02_test> 8)
add_test(NAME test_9_multiply_scalar_parallel COMMAND mpiexec -n 4 $<TARGET_FILE:HPC_02_test> 9)
add_test(NAME test_10_power_parallel COMMAND mpiexec -n 4 $<TARGET_FILE:HPC_02_test> 10)
add_test(NAME test_11_problem_parallel COMMAND mpiexec -n 4 $<TARGET_FILE:HPC_02_test> 11)

# benchmarking
add_executable(HPC_02_benchmark benchmarks/benchmark.cpp ${SOURCE_FILES})
target_include_directories(HPC_02_benchmark PRIVATE include)
target_link_libraries(HPC_02_benchmark PRIVATE MPI::MPI_CXX)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
