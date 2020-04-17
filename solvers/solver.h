//
// Created by Liam Heeger on 2020-04-16.
//

#ifndef SATSOLVER_SOLVER_H
#define SATSOLVER_SOLVER_H

#include <assignment.h>
#include <clause_list.h>


namespace sat_solver {

class Solver {
    public:
        virtual SATState check(const ClauseList& clause_list, Assignment* assignment) = 0;
};

}

#endif //SATSOLVER_SOLVER_H
