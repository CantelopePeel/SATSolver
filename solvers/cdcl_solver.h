#ifndef SATSOLVER_CDCL_SOLVER_H
#define SATSOLVER_CDCL_SOLVER_H

#include "solver.h"
#include <vector>
#include <unordered_set>

namespace sat_solver {

class CDCLSolver : public Solver {
    public:
        SATState check(const ClauseList& clause_list, Assignment* assignment) override;

        SATState backtrack_call(int depth);
    private:
        std::vector<std::vector<int>> mini_clause_list;
        std::vector<int> clause_satisfied_literals;
        std::vector<int> clause_unassigned_literals;

        std::vector<std::vector<int>> variable_to_clause;
        std::vector<int> variable_assignment;
        std::vector<int> variable_assignment_depth;
        int variable_count;
        int starting_clause_list_size;

        bool unit_propagate(int depth, std::unordered_set<int> &propagated_literals);
        void initialize(const ClauseList& clause_list);
        bool get_unassigned_variable(int* variable, int depth);
        void set_literal(int lit, int depth);
        void unset_literal(int lit);
        SATState determine_sat_state(int depth);
};

}


#endif //SATSOLVER_CDCL_SOLVER_H
