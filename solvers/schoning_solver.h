#ifndef SATSOLVER_SCHONING_SOLVER_H
#define SATSOLVER_SCHONING_SOLVER_H

#include "solver.h"

namespace sat_solver {

class SchoningSolver : public Solver {
    public:
        SATState check(const ClauseList& clause_list, Assignment* assignment) override;
        void flip(const ClauseList &clause_list, Assignment* assignment);

};

}


#endif // SATSOLVER_SCHONING_SOLVER_H   
