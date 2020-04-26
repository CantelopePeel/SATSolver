#ifndef SATSOLVER_IMPLICATION_GRAPH_H
#define SATSOLVER_IMPLICATION_GRAPH_H

#include <deque>
#include "clause_list.h"

namespace sat_solver {

class ImplicationGraph {
    public:
        ImplicationGraph(const ClauseList& initial_clause_list);

        void unit_propagate(Assignment* assignment,
                            std::vector<Literal>& assigned_literals,
                            const Clause*& rollback_clause);
        void add_edge(const Literal& start, const Literal& end);
        bool find_unsatisfiable_clause(const ClauseList& clause_list, Assignment* assignment, const Clause*& return_clause);
        bool find_unsatisfiable_clause(const ClauseList& clause_list, Assignment* assignment);
    private:
        class Node {
            public:
                Node(Literal lit, bool implied_or_assigned) : lit_(lit),
                                                              implied_or_assigned_(implied_or_assigned) {};
                const std::deque <Node*>& outgoing_adjacency_list() const { return outgoing_adjacency_list_; };
                const std::deque <Node*>& incoming_adjacency_list() const { return incoming_adjacency_list_; };

                Literal lit_;
                bool implied_or_assigned_;
                std::deque<Node*> outgoing_adjacency_list_;
                std::deque<Node*> incoming_adjacency_list_;
        };

        ClauseList clauses_;
        std::vector<Node> nodes_;
        Node& get_node_of_literal(const Literal& l);

};

}


#endif //SATSOLVER_IMPLICATION_GRAPH_H
