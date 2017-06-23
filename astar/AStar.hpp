#pragma once

#include <geometry/Point.hpp>
#include <functional>

class AStar
{
    public:
        struct Pos
        {
            Pos();
            Pos(int x, int y);

            double dist(const Pos &other) const;

            int x;
            int y;

            inline bool operator<(const Pos &other) const {
                if (x == other.x) {
                    return y < other.y;
                } else {
                    return x < other.x;
                }
            }
        };

        struct Box
        {
            double reachScore;
            double score;
            Pos pos;
            Pos parent;

            inline bool operator<(const Box &other) const {
                if (score == other.score) {
                    return pos < other.pos;
                } else {
                    return score < other.score;
                }
            }
        };

        static std::vector<Pos> solve(
                Pos start, 
                Pos goal,
                std::function<bool(Pos)> reachable,
                size_t maxIter = 0
                );
        
        static std::vector<Point> solveCont(
                Point start,
                Point goal,
                std::function<bool(Point)> reachable,
                double step,
                size_t maxIter = 0
                );
};
