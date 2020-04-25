#ifndef SATSOLVER_IMPLICATION_GRAPH_H
#define SATSOLVER_IMPLICATION_GRAPH_H

#include <deque>
#include "clause_list.h"

namespace sat_solver {

class ImplicationGraph {
    public:
        ImplicationGraph(const std::unordered_set<Variable>& variables);

        void unit_propagate(const ClauseList& clause_list, Assignment* assignment,
                            std::stack<Literal>* assigned_literals,
                            std::unordered_set<Variable>* rollback_variables);
    private:
        class Node {
            public:
                Node(Literal lit) : lit_(lit) {};
                const std::deque <Node*>& outgoing_adjacency_list() const { return outgoing_adjacency_list_; };
                const std::deque <Node*>& incoming_adjacency_list() const { return incoming_adjacency_list_; };

            private:
                Literal lit_;
                std::deque<Node*> outgoing_adjacency_list_;
                std::deque<Node*> incoming_adjacency_list_;
        };

        std::vector<Node> vertices_;

};

}


#endif //SATSOLVER_IMPLICATION_GRAPH_H
