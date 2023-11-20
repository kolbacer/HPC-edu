cmake_minimum_required(VERSION 3.5)
project(HPC_01_OpenMP VERSION 0.1.0 LANGUAGES C CXX)

include(CTest)
enable_testing()

file(GLOB_RECURSE SOURCE_FILES src/*.cpp)

find_package(OpenMP REQUIRED)
add_executable(HPC_01_OpenMP main.cpp ${SOURCE_FILES})

target_include_directories(HPC_01_OpenMP PRIVATE include)
target_link_libraries(HPC_01_OpenMP PRIVATE OpenMP::OpenMP_CXX)

# testing
file(GLOB_RECURSE TEST_FILES tests/*.cpp)
add_executable(HPC_01_test ${TEST_FILES} ${SOURCE_FILES})
target_include_directories(HPC_01_test PRIVATE include)
target_link_libraries(HPC_01_test PRIVATE OpenMP::OpenMP_CXX)

add_test(test_1_add HPC_01_test 1)
add_test(test_2_multiply HPC_01_test 2)
add_test(test_3_trace HPC_01_test 3)
add_test(test_4_atrace HPC_01_test 4)
add_test(test_5_power HPC_01_test 5)
add_test(test_6_multiply_scalar HPC_01_test 6)
add_test(test_7_add_parallel HPC_01_test 7)
add_test(test_8_multiply_parallel HPC_01_test 8)
add_test(test_9_multiply_scalar_parallel HPC_01_test 9)
add_test(test_10_power_parallel HPC_01_test 10)

# benchmarking
add_executable(HPC_01_benchmark benchmarks/benchmark.cpp ${SOURCE_FILES})
target_include_directories(HPC_01_benchmark PRIVATE include)
target_link_libraries(HPC_01_benchmark PRIVATE OpenMP::OpenMP_CXX)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
