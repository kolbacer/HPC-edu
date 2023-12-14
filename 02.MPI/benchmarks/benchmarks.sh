#!/bin/bash

benchmark="./HPC_02_benchmark"

if [[ "$1" != "" ]]; then
    benchmark="$1"
fi

k=3                                    # iterations per task
sizes=(100 1000 2000)                  # matrix sizes
processes=(1 2 3 4 5 6 7 8 9 10 11 12) # num of processes

echo "time unit: ms"
echo "iteratons per task: ${k}"
echo "matrix sizes (vertical): ${sizes[*]}"
echo "num of processes (horizontal): ${processes[*]}"
echo
echo "benchmarks start...."
echo

for n in "${sizes[@]}"; do
    for proc in "${processes[@]}"; do
        mpiexec -n "${proc}" "${benchmark}" "${k}" "${n}"
        printf ' '
    done
    echo
done

echo
echo "benchmarks end"