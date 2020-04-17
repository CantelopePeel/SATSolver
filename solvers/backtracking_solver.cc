//
// Created by Liam Heeger on 2020-04-17.
//

#include <iostream>
#include "backtracking_solver.h"

using namespace sat_solver;

static bool
get_unassigned_variable(const ClauseList& clause_list, const Assignment& assignment, Variable* variable) {
    for (Variable var : clause_list.variables()) {
        if (!assignment.contains(var)) {
            *variable = var;
            return true;
        }
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

static SATState
backtrack_call(const ClauseList& clause_list, Assignment* assignment) {
    SATState sat_state = determine_sat_state(clause_list, *assignment);
    if (sat_state == SATState::UNSAT) {
        // Backtrack!
        return sat_state;
    } else if (sat_state == SATState::UNDEF){
        Variable var(0);
        if (get_unassigned_variable(clause_list, *assignment, &var)) {
            Literal pos_literal(false, var);
            assignment->push_literal(pos_literal);
            SATState pos_sat_state = backtrack_call(clause_list, assignment);
            if (pos_sat_state == SATState::SAT) {
                return pos_sat_state;
            }
            assignment->pop_literal(pos_literal);

            Literal neg_literal(true, var);
            assignment->push_literal(neg_literal);
            SATState neg_sat_state = backtrack_call(clause_list, assignment);
            if (neg_sat_state == SATState::SAT) {
                return neg_sat_state;
            }
            assignment->pop_literal(neg_literal);

            return SATState::UNSAT;
        } else {
            // Should be impossible to get here.
            return SATState::UNSAT;
        }
    } else if (sat_state == SATState::SAT){
        return sat_state;
    } else {
        // Again impossible to get here.
        return SATState::UNSAT;
    }
}

SATState
BacktrackingSolver::
check(const ClauseList& clause_list, Assignment* assignment) {
    SATState is_sat = backtrack_call(clause_list, assignment);
    return is_sat;
}


