#include "implication_graph.h"

using namespace sat_solver;

ImplicationGraph::
ImplicationGraph(const std::unordered_set<Variable>& variables){
    for(const Variable& var : variables){
        Literal pos_lit(true, var);
        nodes_.push_back(Node(pos_lit));
        Literal neg_lit(true, var);
        nodes_.push_back(Node(neg_lit));
    }
}

bool
get_single_unassigned_lit_in_clause(const Clause& clause, Assignment* assignment, Literal*& unassigned_lit){
    int unassigned_literals_found = 0;
    bool is_satisfied = false;
    for(const Literal& lit: clause.literals()){
        if(assignment->contains(lit)){
            is_satisfied = true;
        } else if(!assignment->contains(lit.to_variable())){
            unassigned_literals_found++;
            unassigned_lit = (Literal*) &lit;
        }
    }

    return (!is_satisfied and unassigned_literals_found == 1);
}

ImplicationGraph::Node&
ImplicationGraph::
get_node_of_literal(const Literal& l){
    return nodes_[l.value()];
}

void
ImplicationGraph::
add_edge(const Literal& start, const Literal& end){
    Node& start_node = get_node_of_literal(start);
    Node& end_node = get_node_of_literal(end);

    start_node.outgoing_adjacency_list_.push_back(&end_node);
    end_node.incoming_adjacency_list_.push_back(&start_node);
}

void
ImplicationGraph::
unit_propagate(const ClauseList& clause_list, Assignment* assignment,
               std::stack<Literal>* assigned_literals,
               std::unordered_set<Variable>* rollback_variables){
    bool new_propagation = false;
    do {
        new_propagation = false;
        const Clause* propagated_clause;
        Literal* single_literal = NULL;

        for (const Clause& clause : clause_list.clauses()) {
            if(get_single_unassigned_lit_in_clause(clause, assignment, single_literal)){
                new_propagation = true;
                propagated_clause = &clause;
                break;
            }
        }

        if(new_propagation){
            assignment->push_literal(*single_literal);
            assigned_literals->push(*single_literal);

            for(const Literal& lit : propagated_clause->literals()){
                if(lit.value() != single_literal->value()){
                    add_edge(lit, *single_literal);
                }
            }

            if(
                    assignment->contains(*single_literal) &&
                    assignment->contains(single_literal->negate())
            ){
                // A collision has occurred, and a cut clause will be added.
                Clause new_clause;

                for(const Node* node : get_node_of_literal(*single_literal).incoming_adjacency_list()){
                    new_clause.add_literal(node->lit_.negate());
                }
                for(const Node* node : get_node_of_literal(single_literal->negate()).incoming_adjacency_list()){
                    new_clause.add_literal(node->lit_.negate());
                }

                clause_list.add_clause(new_clause);
            }
        }


    } while(new_propagation);
}
