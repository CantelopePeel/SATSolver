//
// Created by Liam Heeger on 2020-04-16.
//

#ifndef SATSOLVER_CLAUSE_LIST_H
#define SATSOLVER_CLAUSE_LIST_H

#include "assignment.h"
#include "clause.h"
#include <vector>

namespace sat_solver {

class ClauseList {
    public:
        ClauseList();
        void add_clause(Clause clause);
        void add_variable(const Variable& variable);
        bool validate_clause_list();
        bool check_satisfied(const Assignment& assignment) const;

        const std::vector <Clause>& clauses() const { return clauses_; };
        const std::unordered_set <Variable>& variables() const { return variables_; };

        const unsigned long num_clauses() const { return clauses_.size(); };
        const unsigned long num_variables() const { return variables_.size(); };

    protected:
        std::vector<Clause> clauses_;
        std::unordered_set<Variable> variables_;
};

}

#endif //SATSOLVER_CLAUSE_LIST_H
