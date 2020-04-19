#include <iostream>
#include <fstream>

#include "util.h"
#include "clause_list.h"
#include "solvers/backtracking_solver.h"
#include "solvers/cdcl_solver.h"

using namespace sat_solver;

int main(int argc, char* argv[]) {
    if (argc != 2) {
        return 1;
    }

    std::ifstream file_stream;
    file_stream.open(argv[1], std::ios_base::in);

    if (file_stream.is_open()) {
        ClauseList clause_list = util::read_dimacs(file_stream);
        CDCLSolver solver;

        Assignment assignment;
        SATState is_sat = solver.check(clause_list, &assignment);

        if (is_sat == SAT) {
            std::cout << "SAT" << std::endl;
        } else if (is_sat == UNSAT) {
            std::cout << "UNSAT" << std::endl;
        } else if (is_sat == UNDEF) {
            std::cout << "UNDEF" << std::endl;
        }
    } else {
        return 1;
    }

    return 0;
}