#include <iostream>
#include <fstream>

#include "util.h"
#include "clause_list.h"
#include "solvers/backtracking_solver.h"

using namespace sat_solver;

void print_usage() {
    std::printf("Program usage:");
    std::printf("./sat_solvers file <filename>\n");
    std::printf("./sat_solvers gen <num_vars> <num_clauses> <k_sat> <runs>\n");
}

std::string sat_state_str(const SATState& sat_state) {
    switch (sat_state) {
        case SAT:
            return "SAT";
        case UNSAT:
            return "UNSAT";
        case UNDEF:
            return "UNDEF";
    }

    return "error";
}

int run_file_mode(int argc, char* argv[]) {
    if (argc < 3) {
        print_usage();
        return 1;
    }

    std::ifstream file_stream;
    file_stream.open(argv[1], std::ios_base::in);

    if (file_stream.is_open()) {
        ClauseList clause_list = util::read_dimacs(file_stream);
        BacktrackingSolver solver;

        Assignment assignment;
        SATState is_sat = solver.check(clause_list, &assignment);
        std::cout << sat_state_str(is_sat) << std::endl;
    } else {
        std::printf("Could not open file.\n");
        return 1;
    }

    return 0;
}

int run_gen_mode(int argc, char* argv[]) {
    if (argc < 6) {
        print_usage();
        return 1;
    }

    int num_vars = static_cast<int>(std::strtol(argv[2], nullptr, 10));
    int num_clauses = static_cast<int>(std::strtol(argv[3], nullptr, 10));
    int k_sat = static_cast<int>(std::strtol(argv[2], nullptr, 10));
    int runs = std::strtol(argv[2], nullptr, 10);

}


int main(int argc, char* argv[]) {
    if (argc < 2) {
        print_usage();
        return 1;
    }

    std::string mode(argv[1]);
    if (mode == "file") {
        return run_file_mode(argc, argv);
    } else if (mode == "gen") {
        return run_gen_mode(argc, argv);
    } else {
        print_usage();
        return 1;
    }
}