#ifndef SATSOLVER_SCHONING_SOLVER_H
#define SATSOLVER_SCHONING_SOLVER_H

#include "solver.h"

namespace sat_solver {

class schoningSolver : public Solver {
    public:
        SATState check(const ClauseList& clause_list, Assignment* assignment) override;

};

}


#endif // SATSOLVER_SCHONING_SOLVER_H   
