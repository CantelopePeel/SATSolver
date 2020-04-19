#include <iostream>
#include <implication_graph.h>
#include "cdcl_solver.h"

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
backtrack_call(const ClauseList& clause_list, Assignment* assignment,
               ImplicationGraph* implication_graph,
               std::unordered_set<Variable>* rollback_variables) {
    SATState sat_state = determine_sat_state(clause_list, *assignment);
    if (sat_state == SATState::UNSAT) {
        // Backtrack!
        return sat_state;
    } else if (sat_state == SATState::UNDEF){
        Variable var(0);

        if (get_unassigned_variable(clause_list, *assignment, &var)) {
            Literal literal_choices[] = {Literal(false, var), Literal(true, var)};
            std::stack<Literal> assigned_literals;

            for(Literal& lit : literal_choices){
                while(!assigned_literals.empty()){
                    assigned_literals.pop();
                }
                assignment->push_literal(lit);
                assigned_literals.push(lit);

                implication_graph->unit_propagate(clause_list, assignment, &assigned_literals, rollback_variables);

                if(rollback_variables->count(var)){
                    rollback_variables->erase(var);
                }

                if(!rollback_variables->empty()){
                    assignment->pop_literals(assigned_literals);
                    return SATState::UNSAT;
                }

                SATState sat_state = backtrack_call(clause_list, assignment, implication_graph, rollback_variables);

                if (sat_state == SATState::SAT) {
                    return sat_state;
                }

                assignment->pop_literals(assigned_literals);
            }

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
CDCLSolver::
check(const ClauseList& clause_list, Assignment* assignment) {
    ImplicationGraph* implication_graph = new ImplicationGraph(clause_list.variables());
    std::unordered_set<Variable> rollback_variables;
    std::stack<Literal> assigned_literals;
    // propagate unary constraints
    implication_graph->unit_propagate(clause_list, assignment, &assigned_literals, &rollback_variables);

    if(!rollback_variables.empty()){
        // conflict here means both (P = true) and (P = false) occur as unary clauses.
        assignment->pop_literals(assigned_literals);
        return SATState::UNSAT;
    }

    SATState is_sat = backtrack_call(clause_list, assignment, implication_graph, &rollback_variables);

    // the literal popping here means that if the clauses are unsatisfiable, the assignment returned
    // will have the same state at function call and function return.
    if(is_sat == SATState::UNSAT){
        assignment->pop_literals(assigned_literals);
    }

    return is_sat;
}
