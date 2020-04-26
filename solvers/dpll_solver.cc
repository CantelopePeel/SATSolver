//
// Implementation of DPLL Algorithm by Chris Kjellqvist.
// https://en.wikipedia.org/wiki/DPLL_algorithm
//

#include "dpll_solver.h"
#include <unordered_set>
#include <algorithm>
#include <cassert>
#include <iostream>

#define DEBUG 0
using namespace sat_solver;

bool check_satisfied(const std::vector<Clause> clauses_,
    const Assignment* assignment) {
  bool clause_satisfied_flag = true;
  for (const Clause& clause : clauses_) {
    bool satisfied_flag = false;
    for (const Literal& literal : clause.literals()) {
      if (assignment->contains(literal)) {
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

static bool
is_empty(const Clause &c){
  return c.size() == 0;
}

static bool
simplify_clauses(std::vector<Clause> &cll,
    Literal l){
  cll.erase(std::remove_if(cll.begin(), cll.end(), [l](Clause &cl) {
        return cl.contains(l);
        }), cll.end());
  for (Clause &c: cll){
#if DEBUG
    std::cout << "\tConsidering clause: " << c.to_str() << "\n";
#endif
    c.remove_literal(l.negate());
    if (c.size() == 0) {
#if DEBUG
      std::cout << "\t\tEMPTY after simpl\n";
#endif
      return false;
    }
  }
  // remove any clauses that are trivially true by asserting l
  return true;
}

void print(const std::vector<Clause> &clauses) {
  for(Clause c : clauses) {
    std::cout << c.to_str() << " /\\ ";
  }
  std::cout << "True\n";
}
static void
find_pure_literals(std::vector<Clause> &cll,
    std::vector<Literal> &pure_lst) {
  std::unordered_set<Literal> lset;
  std::unordered_set<Variable> dirty_set;

  // go through every literal in the equation and
  // find all the pure ones
  for(Clause &cl: cll){
    for(Literal l: cl.literals()){
      Variable v = l.to_variable();
      if (lset.find(l.negate()) != lset.end()){
        dirty_set.insert(l.to_variable());
        lset.erase(l.negate());
        continue;
      }
      if (dirty_set.find(l.to_variable()) == dirty_set.end()){
        lset.insert(l);
      }
    }
  }
  for(Literal l: lset)
    pure_lst.push_back(l);
}



SATState
DPLLSolver::
check_call(const std::vector<Clause> &clause_list,
    Assignment* assignment) {
  std::vector<Clause> cll(clause_list);
  if (clause_list.size() == 0) return SAT;
  if (check_satisfied(cll, assignment) == SAT)
    return SAT;
  std::vector<Literal> ll;
  bool made_changes = true;
  while (made_changes){
#if DEBUG
    print(clause_list);
#endif
    made_changes = false;
    for (Clause &cl: cll){
      switch(cl.size()){
        case 0:
          // if we have an empty assignment, this is unsat
#if DEBUG
          std::cout << "Found empty clause\n";
#endif
          return UNSAT;
        case 1:
          // if we have a singleton clause, force the assignment
          Literal l = *cl.literals().begin();
          ll.push_back(l);
#if DEBUG
          std::cout << "Resolving singleton clause " << l.to_str() << "\n";
#endif
          if (simplify_clauses(cll, l) == false) return UNSAT;
          made_changes = true;
          goto break_out;
      }
    }
break_out:;
  }

  std::vector<Literal> pures;
  find_pure_literals(cll, pures);
  for(Literal l: pures) {
    auto pur_filter = [l](Clause &c) { return c.literals().find(l) != c.literals().end(); };
    cll.erase(std::remove_if(cll.begin(), cll.end(), pur_filter), cll.end());
    ll.push_back(l);
#if DEBUG
    std::cout << l.to_str() << " is pure\n";
#endif
  }
  Literal next_literal = *((*(cll.begin())).literals().begin());
#if DEBUG
  std::cout << "Choosing literal " << next_literal.to_str() << "\n";
#endif

  std::vector<Clause> cpy = cll;
  simplify_clauses(cpy, next_literal);

  if (check_call(cpy, assignment) == SAT){
    for (Literal l: ll) { 
        assignment->push_literal(l);
        increment_decision_counter();
    }
    return SAT;
  }

#if DEBUG
  std::cout << "Choosing literal " << next_literal.negate().to_str() << "\n";
#endif
  cpy = cll;
  simplify_clauses(cpy, next_literal.negate());

  if (check_call(cpy, assignment) == SAT){
    for (Literal l: ll) { 
        assignment->push_literal(l);
        increment_decision_counter();
    }
    return SAT;
  }

  return UNSAT;
}

SATState
DPLLSolver::
check(const sat_solver::ClauseList& clause_list, sat_solver::Assignment* assignment) {
  assert(clause_list.num_clauses() != 0);
  return check_call(clause_list.clauses(), assignment);
}
