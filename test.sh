#!/bin/zsh

benchmarks=(uf20-91 uf50-218 uf75-325 uf100-430 uf125-538 uf150-645 uf175-753 uf200-860 uf225-960 uf250-1065)
for n in $benchmarks
do
    for n2 in ../cnf/$n/**/*.cnf
    do
        echo $n2 $(./sat_solvers file $n2)
    done
done