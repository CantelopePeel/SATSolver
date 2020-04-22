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


static bool
get_unsatisfied_clause(const ClauseList &clause_list, const Assignment &assignment, Variable *variable)
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
    const long unsigned int number_of_clauses = clause_list.num_clauses();
    //while there is an unassigned variable in the clause list, reassign it randomly between true and false.
    Variable var(0);
    while (get_unassigned_variable(clause_list, *assignment, &var)) {
        if(rand()%2){
            Literal pos_literal(false, var);
            assignment->push_literal(pos_literal);
        }else{
            Literal neg_literal(true, var);
            assignment->push_literal(neg_literal);
        }
    }
    for(int i = 0; i < number_of_clauses*3; i++){
        SATState sat_state = determine_sat_state(clause_list, *assignment);
        if (sat_state == SATState::SAT){
            return sat_state;
        }
    }
    
    return UNDEF;
}