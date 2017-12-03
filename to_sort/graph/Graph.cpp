#include "Graph.hpp"

void Graph::add(Node node)
{
    nodes.push_back(node);
}

void Graph::connect(Node node1, Node node2, double weight)
{
    Edge edge;
    edge.node1 = node1;
    edge.node2 = node2;
    edge.weight = weight;

    edges[node1].push_back(edge);
    edges[node2].push_back(edge);
}


