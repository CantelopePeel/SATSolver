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

        inline void increment_decision_counter() { decision_counter_++; };
        inline const unsigned int decision_counter() { return decision_counter_; };
    protected:
        unsigned int decision_counter_ = 0;
};

}

#endif //SATSOLVER_SOLVER_H
