#!/bin/bash

# Create a folder where we'll store the temporary files
mkdir build
cd build
cmake ../
make

# Enable ** support
shopt -s globstar

# Print out the csv header
echo 'folder_name,file_name,dpll_is_sat,dpll_dec,dpll_time,cdcl_is_sat,cdcl_dec,cdcl_time,scho_is_sat,scho_dec,scho_time' > uf_run.csv

# These are all the uf benchmarks we'll be using.
benchmarks=( uf20-91 uf50-218 uf75-325 uf100-430 uf125-538 uf150-645 uf175-753 )

# This is where the files will be downloaded from.
base_url="https://www.cs.ubc.ca/~hoos/SATLIB/Benchmarks/SAT/RND3SAT/"
file_extension=".tar.gz"

for n in "${benchmarks[@]}"; do
    # We download the tar file
    wget -P cnf $base_url$n$file_extension
    # We create a place for the extracted files
    mkdir cnf/$n
    # We extract the tar
    tar -xf cnf/$n$file_extension -C cnf/$n
    for n2 in $(ls cnf/$n/**/*.cnf); do
        # We get the output, and add it to uf_run.csv
        echo $n,$(./sat_solvers file $n2) >> uf_run.csv
    done
done

# Using the benchmarks so far, generate diagrams.
python3 ../analyze_uf.py uf_run.csv