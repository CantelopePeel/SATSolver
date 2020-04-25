#include "implication_graph.h"

using namespace sat_solver;

ImplicationGraph::
ImplicationGraph(const std::unordered_set<Variable>& variables){
    // for(const Variable& var : variables){
    //     Literal pos_lit(true, var);
    //     vertices_.push_back(Node(pos_lit));
    //     Literal neg_lit(true, var);
    //     vertices_.push_back(Node(neg_lit));
    // }
}

void
ImplicationGraph::
unit_propagate(const ClauseList& clause_list, Assignment* assignment,
               std::stack<Literal>* assigned_literals,
               std::unordered_set<Variable>* rollback_variables){
    // TODO
}