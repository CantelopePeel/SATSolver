#include <iostream>
#include <fstream>
#include <random>

#include "util.h"
#include "clause_list.h"
#include "solvers/backtracking_solver.h"
#include "solvers/cdcl_solver.h"

using namespace sat_solver;

void print_usage() {
    std::printf("Program usage:\n");
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
    file_stream.open(argv[2], std::ios_base::in);

    if (file_stream.is_open()) {
        ClauseList clause_list = util::read_dimacs(file_stream);
        CDCLSolver solver;

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
    int k_sat = static_cast<int>(std::strtol(argv[4], nullptr, 10));
    int trials = static_cast<int>(std::strtol(argv[5], nullptr, 10));

    std::mt19937 rand_num_gen(42);

    int num_cases = num_vars * num_clauses * trials;
    int case_counter = 0;
    std::printf("Generated cases: %d\n", num_cases);
    std::ofstream outfile_stream;
    outfile_stream.open("gen_run.csv");
    if (!outfile_stream.is_open()) {
        std::printf("Could not open output file.\n");
        return 1;
    }

    outfile_stream << "case_num,num_var,num_clause,k_sat,trial,bt_dec,bt_state,cdcl_dec,cdcl_state,dpll_dec,dpll_state,"
                      "scho_dec,scho_state\n";

    for (int num_var = 1; num_var < num_vars + 1; num_var++) {
        std::vector<Literal> literals;
        ClauseList var_clause_list;
        for (int i = 0; i < num_var; i++) {
            auto var = Variable(i);
            var_clause_list.add_variable(var);
            literals.push_back(Literal(false, var));
            literals.push_back(Literal(true, var));
        }

        for (int num_clause = 1; num_clause < num_clauses + 1; num_clause++) {
            for (int trial = 0; trial < trials; trial++) {
                if (case_counter % 10000 == 0) {
                    std::printf("CASE: %d/%d \n", case_counter, num_cases);
                }
                ClauseList trial_clause_list(var_clause_list);
                outfile_stream << case_counter << "," << num_var << "," << num_clause << "," << k_sat << "," << trial << ",";
                case_counter++;

                for (int i = 0; i < num_clause; i++) {
                    std::shuffle(literals.begin(), literals.end(), rand_num_gen);
                    Clause clause;
                    for (int j = 0; j < k_sat; j++) {
                        clause.add_literal(literals[j]);
                    }
                    trial_clause_list.add_clause(clause);
                }

                Assignment bt_assn;
                BacktrackingSolver bt_solver;
                SATState bt_state = bt_solver.check(trial_clause_list, &bt_assn);

                outfile_stream << bt_solver.decision_counter() << "," << sat_state_str(bt_state) << ",";
                outfile_stream << std::endl;
            }
        }
    }
    outfile_stream.close();

    return 0;
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