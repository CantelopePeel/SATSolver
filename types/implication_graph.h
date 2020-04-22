#ifndef SATSOLVER_IMPLICATION_GRAPH_H
#define SATSOLVER_IMPLICATION_GRAPH_H

#include <deque>
#include "clause_list.h"

namespace sat_solver {

class ImplicationGraph {
    private:

    public:
        ImplicationGraph(const std::unordered_set<Variable>& variables);

        void unit_propagate(const ClauseList& clause_list, Assignment* assignment,
                            std::stack<Literal>& assigned_literals,
                            std::unordered_set<Variable>& rollback_variables);
        void add_edge(const Literal& start, const Literal& end);
    private:
        class Node {
            public:
                Node(Literal lit) : lit_(lit) {};
                const std::deque <Node*>& outgoing_adjacency_list() const { return outgoing_adjacency_list_; };
                const std::deque <Node*>& incoming_adjacency_list() const { return incoming_adjacency_list_; };

                Literal lit_;
                std::deque<Node*> outgoing_adjacency_list_;
                std::deque<Node*> incoming_adjacency_list_;
        };

        std::vector<Node> nodes_;
        ClauseList implied_clauses_;
        Node& get_node_of_literal(const Literal& l);

};

}


#endif //SATSOLVER_IMPLICATION_GRAPH_H
