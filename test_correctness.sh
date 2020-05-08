#!/bin/bash

mkdir build
cd build
cmake ../
make

echo Testing for correctness \(an error is thrown if outputs are incorrect between algorithms\)
echo file_name,backtrack_is_sat,backtrack_decisions,backtrack_time,dpll_is_sat,dpll_decisions,dpll_time,cdcl_is_sat,cdcl_decisions,cdcl_time,scho_is_sat,scho_decisions,scho_time
./sat_solvers -b file ../cnf/simple_v3_c2.cnf
./sat_solvers -b file ../cnf/unsat.cnf
./sat_solvers -b file ../cnf/quinn.cnf
./sat_solvers -b file ../cnf/cdcl.cnf
echo file_name,dpll_is_sat,dpll_decisions,dpll_time,cdcl_is_sat,cdcl_decisions,cdcl_time,scho_is_sat,scho_decisions,scho_time
./sat_solvers file ../cnf/aim-50-1_6-yes1-4.cnf
./sat_solvers file ../cnf/aim-100-1_6-no-1.cnf

echo Testing random test cases \(backtracking included\), with up to 30 vars, 30 clauses, 4-SAT, 10 trials
./sat_solvers -b gen 30 30 4 10
echo Testing random test cases \(backtracking included\), with up to 120 vars, 120 clauses, 4-SAT, 5 trials
./sat_solvers gen 120 120 4 5