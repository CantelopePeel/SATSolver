#include <iostream>
#include <cassert>
#include <queue>
#include "cdcl_solver.h"

using namespace sat_solver;
// INF is assumed to be a depth that is unreachable in practice.
#define INF 1000000000

bool
CDCLSolver::
get_unassigned_variable(int* variable, int depth) {
    int best_variable_clauses = 0;

    for(int i = 0; i < variable_count; i++){
        if(variable_assignment[i] == 0 or variable_assignment_depth[i] > depth){
            int curr_variable_clauses = 0;
            for(const int &clause : variable_to_clause[i]){
                if(clause_satisfied_literals[clause] == 0){
                    curr_variable_clauses++;
                }
            }

            // we prefer to choose the variable involved in the most unsatisfied clauses.
            // This is because CDCL "likes" conflicts.
            if(best_variable_clauses < curr_variable_clauses){
                best_variable_clauses = curr_variable_clauses;
                *variable = i;
            }
        }
    }

    return (best_variable_clauses > 0);
}

void
CDCLSolver::
set_literal(int lit, int depth){
    int var = std::abs(lit) - 1;
    assert(variable_assignment[var] == 0 or variable_assignment_depth[var] > depth);

    variable_assignment[var] = (lit > 0) ? 1 : -1;
    variable_assignment_depth[var] = depth;

    for(const int& clause : variable_to_clause[var]){
        for(const int& ite_lit : mini_clause_list[clause]){
            if(ite_lit == lit){
                clause_satisfied_literals[clause]++;
                clause_unassigned_literals[clause]--;
            } else if(ite_lit == -lit){
                clause_unassigned_literals[clause]--;
            }
        }
    }
}

void
CDCLSolver::
unset_literal(int lit){
    int var = std::abs(lit) - 1;
    assert(variable_assignment[var] != 0 and variable_assignment_depth[var] != INF);

    variable_assignment[var] = 0;
    variable_assignment_depth[var] = INF;
    for(const int& clause : variable_to_clause[var]){
        for(const int& ite_lit : mini_clause_list[clause]){
            if(ite_lit == lit){
                clause_satisfied_literals[clause]--;
                clause_unassigned_literals[clause]++;
            } else if(ite_lit == -lit){
                clause_unassigned_literals[clause]++;
            }
        }
    }
}

SATState
CDCLSolver::
determine_sat_state(int depth) {
    for (int i = 0; i < mini_clause_list.size(); i++) {
        if(clause_satisfied_literals[i] > 0){
            continue;
        } else if(clause_unassigned_literals[i] == 0){
            return SATState::UNSAT;
        } else {
            return SATState::UNDEF;
        }
    }

//    // A complete assignment should have all variables assignment to a value.
//    for (const auto& var : clause_list.variables()) {
//        if (!assignment.contains(var)) {
//            return SATState::UNDEF;
//        }
//    }

    return SATState::SAT;
}

bool
CDCLSolver::
unit_propagate(int depth, std::unordered_set<int> &propagated_literals){
    bool change = false;

    std::unordered_set<int> antecedents;
    std::queue<int> inspect;

    for(int i = 0; i < mini_clause_list.size(); i++){
        if(clause_satisfied_literals[i] == 0 and clause_unassigned_literals[i] <= 1){
            inspect.push(i);
        }
    }

    while(!inspect.empty()){
        int cl = inspect.front();
        inspect.pop();

        if(clause_satisfied_literals[cl] > 0 or clause_unassigned_literals[cl] >= 2){
            continue;
        } else if(clause_unassigned_literals[cl] == 0) {
            //conflict!

            std::vector<int> new_clause;

            // The new clause says that to avoid the conflict in the future, some step
            // along the propagation process must fail.
            for(const int &lit : antecedents){
                // We make sure not to include the resolved variable.
                if(antecedents.count(-lit) == 0){
                    new_clause.push_back(lit);
                }
            }

            // since the same conflict clause can be derived from different propagations,
            // we make sure not to add a duplicate.
            bool duplicate = false;
            for(const std::vector<int>& clause : mini_clause_list){
                if(clause == new_clause){
                    duplicate = true;
                    break;
                }
            }

            if(new_clause.size() != 0 and !duplicate){
                mini_clause_list.push_back(new_clause);

                clause_unassigned_literals.push_back(0);
                clause_satisfied_literals.push_back(0);

                for(const int& lit : new_clause){
                    int var = std::abs(lit) - 1;
                    variable_to_clause[var].push_back(mini_clause_list.size() - 1);
                    if(variable_assignment[var] == 0 or variable_assignment_depth[var] > depth){
                        clause_unassigned_literals[mini_clause_list.size() - 1]++;
                    } else if((variable_assignment[var] > 0) == (lit > 0)){
                        clause_satisfied_literals[mini_clause_list.size() - 1]++;
                    }
                }
            }

            for(const int& lit : propagated_literals){
                unset_literal(lit);
            }
            // learn a new clause based on the antecedent literals.

            return true;
        } else {
            // unit propagation!

            int unassigned_lit = 0;
            for(const int& lit : mini_clause_list[cl]){
                int var = std::abs(lit) - 1;
                if(variable_assignment[var] == 0 or variable_assignment_depth[var] > depth){
                    unassigned_lit = lit;
                    break;
                }
            }

            assert(unassigned_lit != 0);
            int var = std::abs(unassigned_lit) - 1;

            // we store all of the literals affecting the propagation in antecedents, so that if
            // we get a conflict later on, we would know the trouble-making literals.
            for(const int& lit : mini_clause_list[cl]){
                antecedents.insert(lit);
            }
            propagated_literals.insert(unassigned_lit);

            set_literal(unassigned_lit, depth);
            for(const int& clause : variable_to_clause[var]){
                inspect.push(clause);
            }
        }
    }

    return false;
}

SATState
CDCLSolver::
backtrack_call(int depth) {
    std::unordered_set<int> propagated_literals;
    bool conflict = unit_propagate(depth, propagated_literals);

    if(conflict){
        return SATState::UNSAT;
    }

    SATState sat_state = determine_sat_state(depth);
    if (sat_state == SATState::UNSAT) {
        for(int lit : propagated_literals){
            unset_literal(lit);
        }
        // Backtrack!
        return sat_state;
    } else if (sat_state == SATState::UNDEF){
        int var;

        if (get_unassigned_variable(&var, depth)) {
            set_literal(var + 1, depth);
            increment_decision_counter();

            SATState sat_state = backtrack_call(depth + 1);
            if(sat_state == SATState::SAT){
                return sat_state;
            }

            unset_literal(var + 1);
            set_literal(-(var + 1), depth);
            sat_state = backtrack_call(depth + 1);
            if(sat_state == SATState::SAT){
                return sat_state;
            }

            unset_literal(-(var + 1));
            for(int lit : propagated_literals){
                unset_literal(lit);
            }
        }

        return SATState::UNSAT;
    } else if (sat_state == SATState::SAT){
        return sat_state;
    } else {
        // Again impossible to get here.
        return SATState::UNSAT;
    }
}

void
CDCLSolver::
initialize(const ClauseList& clause_list){
    variable_count = clause_list.variables().size();
    variable_assignment.resize(variable_count, 0);
    variable_assignment_depth.resize(variable_count, INF);
    mini_clause_list.resize(clause_list.clauses().size());
    clause_satisfied_literals.resize(clause_list.clauses().size());
    clause_unassigned_literals.resize(clause_list.clauses().size());
    starting_clause_list_size = clause_list.clauses().size();
    variable_to_clause.resize(variable_count);

    int i = 0; // we have to use a separte index, as clauses is iterable but not indexable.
    for(const Clause& clause : clause_list.clauses()){
        clause_satisfied_literals[i] = 0;
        clause_unassigned_literals[i] = clause.literals().size();

        for(const Literal& lit : clause.literals()){
            if(lit.sign()){
                mini_clause_list[i].push_back(-((int) lit.to_variable() + 1));
                variable_to_clause[lit.to_variable()].push_back(i);
            } else {
                mini_clause_list[i].push_back((int) lit.to_variable() + 1);
                variable_to_clause[lit.to_variable()].push_back(i);
            }
        }

        i++;
    }
}

SATState
CDCLSolver::
check(const ClauseList& clause_list, Assignment* assignment) {
    initialize(clause_list);
    std::unordered_set<int> propagated_literals;
    bool conflict = unit_propagate(0, propagated_literals);

    if(conflict){
        // conflict here means both (P = true) and (P = false) occur as unary clauses.
        return SATState::UNSAT;
    }

    SATState is_sat = backtrack_call(1);

    if(is_sat == SATState::UNSAT){
        return SATState::UNSAT;
    }

    for(int i = 0; i < variable_count; i++){
        if(variable_assignment[i] != 0 and variable_assignment_depth[i] < INF){
            assignment->push_literal(Literal((variable_assignment[i] < 0), Variable(i)));
        }
    }

    return is_sat;
}
