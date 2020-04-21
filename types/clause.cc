//
// Created by Liam Heeger on 2020-04-16.
//

#include "clause.h"


using namespace sat_solver;

Clause::
Clause() {}

void
Clause::
add_literal(const Literal& literal) {
    literal_set_.insert(literal);
}

bool
Clause::
remove_literal(const Literal& literal) {
    return literal_set_.erase(literal) == 1;
}

// if we resolve x_1 as true, we may want to purge all
// instances of x_1 and ~x_1 from the clause
bool
Clause::
purge_literal(const Literal& literal) {
  return literal_set_.erase(literal) || literal_set_.erase(literal.negate());
}

bool
Clause::
contains(const Literal& literal) const {
    return literal_set_.count(literal) == 1;
}

size_t size() const {
  return literal_set_.size();
}

std::string
Clause::
to_str() const {
    std::string clause_str = "{ ";
    for (const auto& lit : literal_set_) {
        if (lit.sign()) {
            clause_str += "-";
        }
        clause_str += std::to_string(lit.to_variable().value() + 1);
        clause_str += " ";
    }
    clause_str += "}";
    return clause_str;
}

