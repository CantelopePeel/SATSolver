//
// Created by Liam Heeger on 2020-04-16.
//

#ifndef SATSOLVER_ASSIGNMENT_H
#define SATSOLVER_ASSIGNMENT_H

#include <deque>
#include <unordered_set>
#include "literal.h"

namespace sat_solver {

enum SATState {
    UNDEF = 0,
    SAT = 1,
    UNSAT = 2
};

class Assignment {
    public:
        void push_literal(const Literal& literal);
        bool pop_literal(const Literal& literal);
        bool remove_literal(const Literal& literal);

        bool contains(const Literal& literal) const;
        bool contains(const Variable& variable) const;

        inline const std::unordered_set<Literal>& literals() const { return literal_set_; };
        inline const unsigned long size() const { return literal_set_.size(); };

    protected:
        std::unordered_set<Literal> literal_set_;
        std::deque<Literal> trail_;

};

}


#endif //SATSOLVER_ASSIGNMENT_H
