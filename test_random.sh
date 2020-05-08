#!/bin/bash

mkdir build
cd build
cmake ../
make

./sat_solvers gen 120 120 4 5

python3 ../analyze_random.py gen_run.csv