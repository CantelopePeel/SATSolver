//
// Created by Liam Heeger on 2020-04-17.
//

#ifndef SATSOLVER_BACKTRACKING_SOLVER_H
#define SATSOLVER_BACKTRACKING_SOLVER_H

#include "solver.h"

namespace sat_solver {

class BacktrackingSolver : public Solver {
    public:
        SATState check(const ClauseList& clause_list, Assignment* assignment) override;

};

}


#endif //SATSOLVER_BACKTRACKING_SOLVER_H
