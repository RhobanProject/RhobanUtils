#include <iostream>
#include <algorithm>
#include <set>
#include "Djikstra.hpp"

std::vector<Graph::Node> Djikstra::findPath(
        Graph &graph, 
        Graph::Node start, 
        Graph::Node goal,
        double *score
        )
{
    // Maps a node to the next node that should be taken
    std::map<Graph::Node, Graph::Node> path;

    // Distance from the starting node to the goal
    std::map<Graph::Node, double> distances;

    // Explored nodes
    std::set<Graph::Node> toExplore;

    // Initalizing with start edge
    distances[start] = 0;
    toExplore.insert(start);

    while (toExplore.size()) {
        // Select the next node
        // XXX the performance of this can be improved by maintaining
        // a more adapted control structure
        Graph::Node node;
        double smallest = -1;
        for (auto &tmpNode : toExplore) {
            if (smallest < 0 || distances[tmpNode] < smallest) {
                smallest = distances[tmpNode];
                node = tmpNode;
            }
        }

        // Removing the node
        double weight = distances[node];
        toExplore.erase(node);

        // Updating
        for (auto &edge : graph.edges[node]) {
            auto target = ((edge.node1 == node) ? edge.node2 : edge.node1);
            double cost = weight + edge.weight;

            if (!distances.count(target)) {
                toExplore.insert(target);
            }

            if (!distances.count(target) || cost < distances[target]) {
                distances[target] = cost;
                path[target] = node;
            }
        }

        if (distances.count(goal) && !toExplore.count(goal)) {
            break;
        }
    }

    std::vector<Graph::Node> nodes;
    if (path.count(goal)) {
        Graph::Node tmp = goal;
        nodes.push_back(tmp);
        while (tmp != start) {
            tmp = path[tmp];
            nodes.push_back(tmp);
        }
        std::reverse(nodes.begin(), nodes.end());
    }

    if (score != NULL) {
        *score = distances[goal];
    }

    return nodes;
}

