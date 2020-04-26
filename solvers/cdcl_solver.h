#ifndef SATSOLVER_CDCL_SOLVER_H
#define SATSOLVER_CDCL_SOLVER_H

#include "solver.h"
#include "implication_graph.h"

namespace sat_solver {

class CDCLSolver : public Solver {
    public:
        SATState check(const ClauseList& clause_list, Assignment* assignment) override;

        SATState backtrack_call(const ClauseList& clause_list, Assignment* assignment,
                                ImplicationGraph& implication_graph,
                                const Clause*& rollback_clause, int depth = 0);
};

}


#endif //SATSOLVER_CDCL_SOLVER_H
