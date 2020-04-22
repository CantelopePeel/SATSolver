#include "schoning_solver.h"

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
determine_sat_state(const ClauseList &clause_list, const Assignment &assignment)
{
    for (const Clause &clause : clause_list.clauses())
    {
        bool sat_clause = false;
        bool unsat_clause = true;
        for (const Literal &clause_lit : clause.literals())
        {
            auto neg_clause_lit = clause_lit.negate();
            if (assignment.contains(clause_lit))
            {
                // Clause satisfied, check next.
                sat_clause = true;
                break;
            }
            else
            {
                unsat_clause &= assignment.contains(neg_clause_lit);
            }
        }

        if (sat_clause)
        {
            continue;
        }

        if (unsat_clause)
        {
            return SATState::UNSAT;
        }
        else
        {
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
schoningSolver::
    check(const ClauseList &clause_list, Assignment *assignment)
{
    int number_of_clauses = clause_list.num_clauses;

    return UNDEF;
}