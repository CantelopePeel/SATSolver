#include "schoning_solver.h"
#include <iostream>


using namespace sat_solver;

static bool
get_unassigned_variable(const ClauseList &clause_list, const Assignment &assignment, Variable *variable)
{
    for (Variable var : clause_list.variables())
    {
        if (!assignment.contains(var))
        {
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

static void
flip(const ClauseList &clause_list, Assignment* assignment)
{
    //look through all clauses
    for (const Clause &clause : clause_list.clauses())
    {
        bool sat_clause = false;
        bool unsat_clause = true;
        for (const Literal &literal : clause.literals())
        {    
            auto neg_clause_lit = literal.negate();
            if (assignment->contains(literal)) {
                // Clause satisfied, check next.
                sat_clause = true;
                break;
            } else {
                unsat_clause &= assignment->contains(neg_clause_lit);
            }
        }

        if (sat_clause) {
            continue;
        }
        // if the clause is unsatisfied, that means the assignment contained non of the literals in the clause
        // we pick a random literal and push it to the assignment, and remove its negation from the assignment.
        if (unsat_clause) {

            auto stop_point = rand() % (clause.literals().size() + 1);
            int index = 0;
            for (const Literal& literal : clause.literals())
            {
                if (index == stop_point)
                {    
                    //pick a literal and flip its value
                    assignment-> remove_literal(literal.negate());            
                    assignment->push_literal(literal);
                    return;
                }
                index++;

            }

        }
    }
}

SATState
schoningSolver::
    check(const ClauseList &clause_list, Assignment *assignment)
{
    const long unsigned int num_vars = clause_list.num_variables();
    //while there is an unassigned variable in the clause list, reassign it randomly between true and false.
    Variable var(0);

    while (get_unassigned_variable(clause_list, *assignment, &var))
    {
        if (rand() % 2)
        {
            Literal pos_literal(false, var);
            assignment->push_literal(pos_literal);
        }
        else
        {
            Literal neg_literal(true, var);
            assignment->push_literal(neg_literal);
        }
    }

    for (int i = 0; i < num_vars * 80; i++)
    {
        SATState sat_state = determine_sat_state(clause_list, *assignment);
        if (sat_state == SATState::SAT)
        {
            return sat_state;
        }
        else
        {
            //flip a random literal from an unsatisfied clause
            flip(clause_list, assignment);
        }
    }

    return UNDEF;
}