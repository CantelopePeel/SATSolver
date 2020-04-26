#include "implication_graph.h"
#include <cassert>

using namespace sat_solver;

ImplicationGraph::
ImplicationGraph(const ClauseList& clause_list){
    clauses_ = clause_list;

    // Since clauses_.variables() is an unordered set, we need to first initialize nodes_
    // to the proper size, then place the proper literal in each node.
    Node tmp (Literal(false, Variable(0)), false);
    nodes_.resize(2 * clauses_.variables().size(), tmp);
    for(const Variable& var : clauses_.variables()){
        Literal neg_lit(false, var);
        get_node_of_literal(neg_lit).lit_ = neg_lit;
        Literal pos_lit(true, var);
        get_node_of_literal(pos_lit).lit_ = pos_lit;
    }
}

bool
get_single_unassigned_lit_in_clause(const Clause& clause, Assignment* assignment, const Literal*& unassigned_lit){
    int unassigned_literals_found = 0;
    bool is_satisfied = false;
    for(const Literal& lit: clause.literals()){
        if(assignment->contains(lit)){
            is_satisfied = true;
        } else if(!assignment->contains(lit.to_variable())){
            unassigned_literals_found++;
            unassigned_lit = &lit;
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
propagate_clauses(ClauseList& clause_list, Assignment* assignment,
                  const Clause*& prop_clause, const Literal*& assigned_literal){
	for(int i = 0; i < clause_list.clauses().size(); i++){
		const Clause& clause = clause_list.clauses()[i];
        if(get_single_unassigned_lit_in_clause(clause, assignment, assigned_literal)){
            prop_clause = &clause;
            // assigned_literal had its pointed memory value modified in the function call.
            return true;
        }
    }

    return false;
}

bool is_clause_unsatisfiable_wrt_assignment(const Clause& clause, Assignment* assignment){
    for(const Literal& lit : clause.literals()){
        if(assignment->contains(lit)){
            // this means the clause is satisfied.
            return false;
        } else if(!assignment->contains(lit.to_variable())){
            // this means we could add lit as a literal.
            return false;
        }
    }

    return true;
}

bool
ImplicationGraph::
find_unsatisfiable_clause(const ClauseList& clause_list, Assignment* assignment, const Clause*& return_clause){
    for(const Clause& clause : clause_list.clauses()){
        if(is_clause_unsatisfiable_wrt_assignment(clause, assignment)){
            return_clause = &clause;
            return true;
        }
    }
    return false;
}

bool
ImplicationGraph::
find_unsatisfiable_clause(const ClauseList& clause_list, Assignment* assignment){
    const Clause* tmp = NULL;
    return find_unsatisfiable_clause(clause_list, assignment, tmp);
}


void
ImplicationGraph::
unit_propagate(Assignment* assignment,
               std::vector<Literal>& assigned_literals,
               const Clause*& rollback_clause){
    // To make the conflict resolution logic simpler, the function assumes that the partial
    // assignment thus far does not have an unsatisfiable clause
    // (A clause is unsatisfiable w.r.t a partial assignment if all of the variables are
    //  assigned, but the clause is still false).

    assert(!find_unsatisfiable_clause(clauses_, assignment));

    rollback_clause = NULL;
    bool found_new_lit = false;
    do {
        found_new_lit = false;
        const Clause* propagated_clause;
        const Literal* new_lit = NULL;

        found_new_lit = propagate_clauses(
            clauses_, assignment, propagated_clause, new_lit
        );

        if(found_new_lit){
            assignment->push_literal(*new_lit);
            assigned_literals.push_back(*new_lit);

            for(const Literal& lit : propagated_clause->literals()){
                if(lit.value() != new_lit->value()){
                    add_edge(lit.negate(), *new_lit);
                }
            }

            const Clause* unsatisfiable_clause = NULL;
            if(find_unsatisfiable_clause(clauses_, assignment, unsatisfiable_clause)){
                // Finding an unsatisfiable clause now means that a variable p must be both
                // true and false.

                // A collision has occurred, and a cut clause will be added.
                Clause* new_clause = new Clause();

                for(const Literal& lit : unsatisfiable_clause->literals()){
                    if(lit.to_variable() != new_lit->to_variable()){
                        add_edge(lit.negate(), new_lit->negate());
                    }
                }

                // The cut contains all the literals implying the literal (*new_lit),
                for(const Node* node : get_node_of_literal(*new_lit).incoming_adjacency_list()){
                    new_clause->add_literal(node->lit_.negate());
                }

                for(const Node* node : get_node_of_literal(new_lit->negate()).incoming_adjacency_list()){
                    new_clause->add_literal(node->lit_.negate());
                }

                rollback_clause = new_clause;
                for(const Clause& clause : clauses_.clauses()){
                	if(clause.literals() == new_clause->literals()){
                		return;
                	}
                }
                clauses_.add_clause(*new_clause);
                return;
            }
        }
    } while(found_new_lit);
}
