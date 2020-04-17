//
// Created by Liam Heeger on 2020-04-16.
//

#include "assignment.h"

using namespace sat_solver;

void
Assignment::
push_literal(const Literal& literal) {
    trail_.push_back(literal);
    literal_set_.insert(literal);
}

bool
Assignment::
pop_literal(const Literal& literal) {
    if (trail_.back() == literal) {
        trail_.pop_back();
        return literal_set_.erase(literal) == 1;
    } else {
        return false;
    }
}

bool
Assignment::
remove_literal(const Literal& literal) {
    auto trail_iter = std::find(trail_.begin(), trail_.end(), literal);
    if (trail_iter != trail_.end()) {
        trail_.erase(trail_iter);
        return literal_set_.erase(literal) == 1;
    } else {
        return false;
    }
}

bool
Assignment::
contains(const Literal& literal) const {
    return literal_set_.count(literal) == 1;
}

bool Assignment::contains(const Variable& variable) const {
    return contains(Literal(true, variable))
        || contains(Literal(false, variable));
}

