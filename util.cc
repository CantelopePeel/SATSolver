//
// Created by Liam Heeger on 2020-04-16.
//

#include "util.h"

#include <iostream>
#include <vector>
#include <string>

using namespace sat_solver;

static std::vector<std::string> split_string(const std::string& str, const char& delimiter) {
    std::vector<std::string> tokens;
    auto tok_start = str.begin();
    auto tok_end = str.begin();

    while (tok_start != str.end()) {
        tok_end = tok_start;
        tok_start = std::find(tok_start, str.end(), delimiter);

        if (tok_start != str.end()) {
            std::string token(tok_end, tok_start);
            if (token.length() > 0) {
                tokens.push_back(token);
            }
            tok_end = ++tok_start;
        }
    }

    std::string final_token(tok_end, tok_start);
    if (final_token.length() > 0) {
        tokens.push_back(final_token);
    }

    return tokens;
}

ClauseList util::read_dimacs(std::ifstream& input_stream) {
    ClauseList clause_list;
    for (std::string line; std::getline(input_stream, line);) {
        auto tokens = split_string(line, ' ');
        if (tokens[0] == "p") {
            // parsed_program_line = true;
            uint64_t num_variables = std::stoul(tokens[2]);
            // num_clauses = std::stoul(tokens[3]);
            for (int i = 1; i < (num_variables + 1); i++) {
                clause_list.add_variable(Variable(i));
            }
        } else if (tokens[0] == "c") {
            continue;
        } else { // This is a clause.
            Clause clause;
            for (const auto& token : tokens) {
                int lit_value = std::stoi(token);

                if (lit_value == 0) {
                    break;
                }
                Literal lit(lit_value < 0, Variable(std::abs(lit_value)-1));

                clause.add_literal(lit);
            }
            clause_list.add_clause(clause);
        }
    }

    return clause_list;
}


