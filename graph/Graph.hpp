#pragma once

#include <vector>
#include <map>

class Graph
{
    public:
        typedef unsigned int Node;
        
        struct Edge
        {
            Node node1;
            Node node2;
            double weight;
        };

        void add(Node node);
        void connect(Node node1, Node node2, double weight=0);

        std::vector<Node> nodes;
        std::map<Node, std::vector<Edge>> edges;
};
