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

bool
propagate_clauses(const ClauseList& clause_list, Assignment* assignment,
                  const Clause* prop_clause, Literal* assigned_literal){
    for (const Clause& clause : clause_list.clauses()) {
        if(get_single_unassigned_lit_in_clause(clause, assignment, assigned_literal)){
            prop_clause = &clause;
            // assigned_literal had its pointed memory value modified in the function call.
            return true;
        }
    }

    return false;
}

void
ImplicationGraph::
unit_propagate(const ClauseList& clause_list, Assignment* assignment,
               std::stack<Literal>& assigned_literals,
               std::unordered_set<Variable>& rollback_variables){
    bool found_new_lit = false;
    do {
        found_new_lit = false;
        const Clause* propagated_clause;
        Literal* new_lit = NULL;

        found_new_lit = propagate_clauses(
            clause_list, assignment, propagated_clause, new_lit
        );
        if(!found_new_lit){
            found_new_lit = propagate_clauses(
                implied_clauses_, assignment, propagated_clause, new_lit
            );
        }

        if(found_new_lit){
            assignment->push_literal(*new_lit);
            assigned_literals.push(*new_lit);

            for(const Literal& lit : propagated_clause->literals()){
                if(lit.value() != new_lit->value()){
                    add_edge(lit, *new_lit);
                }
            }

            if(
                assignment->contains(*new_lit) &&
                assignment->contains(new_lit->negate())
            ){
                // A collision has occurred, and a cut clause will be added.
                Clause new_clause;

                for(const Node* node : get_node_of_literal(*new_lit).incoming_adjacency_list()){
                    new_clause.add_literal(node->lit_.negate());
                }
                for(const Node* node : get_node_of_literal(new_lit->negate()).incoming_adjacency_list()){
                    new_clause.add_literal(node->lit_.negate());
                }

                for(const Literal& variable : new_clause.literals()){
                    rollback_variables.insert(variable.to_variable());
                }

                implied_clauses_.add_clause(new_clause);
            }
        }
    } while(found_new_lit);
}
