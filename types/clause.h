//
// Created by Liam Heeger on 2020-04-16.
//

#ifndef SATSOLVER_CLAUSE_H
#define SATSOLVER_CLAUSE_H

#include "literal.h"
#include <unordered_set>
#include <string>

namespace sat_solver {

class Clause {
    public:
        Clause();
        void add_literal(const Literal& literal);
        bool remove_literal(const Literal& literal);

        bool contains(const Literal& literal) const;
        bool contains(const Variable& variable) const;

        inline const std::unordered_set<Literal>& literals() const { return literal_set_; };

        std::string to_str() const;

    protected:
        std::unordered_set<Literal> literal_set_;
};

}
#endif //SATSOLVER_CLAUSE_H
