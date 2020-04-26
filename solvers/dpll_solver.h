//
// Created by Liam Heeger on 2020-04-18.
//

#ifndef SATSOLVER_DPLL_SOLVER_H
#define SATSOLVER_DPLL_SOLVER_H

#include "solver.h"

namespace sat_solver {

class DPLLSolver : public Solver {
    public:
        SATState check(const ClauseList& clause_list, Assignment* assignment) override;
        SATState check_call(const std::vector<Clause>& clause_list, Assignment* assignment);
};

}


#endif //SATSOLVER_DPLL_SOLVER_H
