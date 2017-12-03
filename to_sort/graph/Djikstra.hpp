#pragma once

#include "Graph.hpp"

class Djikstra
{
    public:

        static std::vector<Graph::Node> findPath(
                Graph &graph, 
                Graph::Node start, 
                Graph::Node goal,
                double *score = NULL
                );
};
