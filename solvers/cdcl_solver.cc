#include <iostream>
#include <cassert>
#include "cdcl_solver.h"

using namespace sat_solver;

static bool
get_unassigned_variable(const ClauseList& clause_list, const Assignment& assignment, Variable* variable) {
    for(const Clause& clause : clause_list.clauses()){
        for(const Literal& lit : clause.literals()){
            if(!assignment.contains(lit.to_variable())){
                *variable = lit.to_variable();
                return true;
            }
        }
    }
    return false;
}

static void
pop_literals(Assignment* assignment, std::vector<Literal>& recent_literals){
    while(!recent_literals.empty()){
        Literal& lit = recent_literals.back();
        assignment->pop_literal(lit);
        recent_literals.pop_back();
    }
}

static bool should_continue_backtracking(const Clause* clause, Assignment* assignment, std::vector<Literal> recent_literals, ImplicationGraph& implication_graph){
    // We want to backtrack non-chronologically as much as we can, while still
    // being able to unit propagate the clause.
    // So the conditions for stopping backtracking are:
    //     1. We should be able to unit propagate
    //     2. If we remove all recent_literals from assignment, then we should not be able to unit propagate.

    int unassigned_literals_found = 0;
    for(const Literal& lit: clause->literals()){
        if(assignment->contains(lit)){
            return false;
        } else if(assignment->contains(lit.negate())){
            unassigned_literals_found++;
        }
    }

    int unassigned_literals_found_before_recent = unassigned_literals_found;
    for(const Literal& lit : recent_literals){
        assert(assignment->contains(lit));

        if(clause->contains(lit.to_variable())){
            unassigned_literals_found_before_recent--;
        }
    }

    for(const Literal& lit : assignment->literals()){
        if(assignment->contains(lit) && assignment->contains(lit.negate())){
            return true;
        }
    }

    if(clause->literals().size() == 1){
        return (unassigned_literals_found == 0);
    }

    return false;
}

static SATState
determine_sat_state(const ClauseList& clause_list, const Assignment& assignment) {
    for (const Clause& clause : clause_list.clauses()) {
        bool sat_clause = false;
        bool unsat_clause = true;
        for (const Literal& clause_lit : clause.literals()) {
            auto neg_clause_lit = clause_lit.negate();
            if (assignment.contains(clause_lit)) {
                // Clause satisfied, check next.
                sat_clause = true;
                break;
            } else {
                unsat_clause &= assignment.contains(neg_clause_lit);
            }
        }

        if (sat_clause) {
            continue;
        }

        if (unsat_clause) {
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

SATState
CDCLSolver::
backtrack_call(const ClauseList& clause_list, Assignment* assignment,
               ImplicationGraph& implication_graph,
               const Clause*& rollback_clause, int depth) {
    std::vector<Literal> recent_literals;
    implication_graph.unit_propagate(assignment, recent_literals, rollback_clause);
    decision_counter_ += recent_literals.size();

    if(rollback_clause != NULL){
        pop_literals(assignment, recent_literals);

        return SATState::UNSAT;
    }

    SATState sat_state = determine_sat_state(clause_list, *assignment);
    if (sat_state == SATState::UNSAT) {
        pop_literals(assignment, recent_literals);
        // Backtrack!
        return sat_state;
    } else if (sat_state == SATState::UNDEF){
        Variable var(0);

        if (get_unassigned_variable(clause_list, *assignment, &var)) {
            Literal literal_choices[] = {Literal(false, var), Literal(true, var)};

            for(int i = 0; i < 2; i++){
                Literal& lit = literal_choices[i];
                assignment->push_literal(lit);
                increment_decision_counter();
                SATState sat_state = backtrack_call(clause_list, assignment, implication_graph, rollback_clause, depth + 1);

                if (sat_state == SATState::SAT) {
                    return sat_state;
                }


                if(rollback_clause != NULL){
                    // One of our descendant function calls reported a conflict.
                    // We inspect the rollback clause, and see if we should stop backtracking
                    // at the current level.

                   if(should_continue_backtracking(rollback_clause, assignment, recent_literals, implication_graph)){
                       assignment->pop_literal(lit);
                       pop_literals(assignment, recent_literals);

                       return SATState::UNSAT;
                   }

                }

                assignment->pop_literal(lit);
            }
        }

        pop_literals(assignment, recent_literals);

        return SATState::UNSAT;
    } else if (sat_state == SATState::SAT){
        return sat_state;
    } else {
        pop_literals(assignment, recent_literals);
        // Again impossible to get here.
        return SATState::UNSAT;
    }
}

SATState
CDCLSolver::
check(const ClauseList& clause_list, Assignment* assignment) {
    ImplicationGraph implication_graph(clause_list);
    const Clause* rollback_clause = NULL;
    std::vector<Literal> recent_literals;

    if(rollback_clause != NULL){
        // conflict here means both (P = true) and (P = false) occur as unary clauses.
        pop_literals(assignment, recent_literals);

        return SATState::UNSAT;
    }

    SATState is_sat = backtrack_call(clause_list, assignment, implication_graph, rollback_clause);

    // the literal popping here means that if the clauses are unsatisfiable, the assignment returned
    // will have the same state at function call and function return.
    if(is_sat == SATState::UNSAT){
        pop_literals(assignment, recent_literals);
    }

    return is_sat;
}
