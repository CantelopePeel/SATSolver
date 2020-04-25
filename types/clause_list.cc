//
// Created by Liam Heeger on 2020-04-16.
//

#include "clause_list.h"
#include "assignment.h"

#include <algorithm>
#include <iostream>

using namespace sat_solver;

ClauseList::ClauseList()
        : clauses_(), variables_() {}

ClauseList::ClauseList(const ClauseList &cll) {
  for (const Clause &cl: cll.clauses_)
    add_clause(cl); // should call copy constructor...
  for (const Variable &v: cll.variables_)
    add_variable(v);
}

void ClauseList::add_clause(Clause clause) {
    for (const Literal& lit : clause.literals()) {
        Variable var = lit.to_variable();
        variables_.insert(var);
    }

    clauses_.push_back(clause);
}

bool ClauseList::validate_clause_list() {
    return false;
}

void ClauseList::add_variable(const Variable& variable) {
    variables_.insert(variable);
}

bool ClauseList::check_satisfied(const Assignment& assignment) const {
    bool clause_satisfied_flag = true;
    for (const Clause& clause : clauses_) {
        bool satisfied_flag = false;
        for (const Literal& literal : clause.literals()) {
            if (assignment.contains(literal)) {
                satisfied_flag = true;
                break;
            }
        }

        if (!satisfied_flag) {
            clause_satisfied_flag = false;
        }
    }
    return clause_satisfied_flag;
}
