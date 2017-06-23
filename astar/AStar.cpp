#include <unistd.h>
#include <cmath>
#include <algorithm>
#include <map>
#include <set>
#include "AStar.hpp"

AStar::Pos::Pos()
{
}

AStar::Pos::Pos(int x, int y)
    : x(x), y(y)
{
}

double AStar::Pos::dist(const Pos &other) const
{
    return sqrt(pow(x-other.x, 2) + pow(y-other.y, 2));
}

std::vector<AStar::Pos> AStar::solve(
        Pos start, 
        Pos goal,
        std::function<bool(Pos)> reachable,
        size_t maxIter,
        double *score
        )
{
    // Boxes
    std::map<Pos, Box> boxes;

    // Queue to explore ( a box
    std::set<Box> queue;

    // Starting point
    AStar::Box startBox;
    startBox.pos = start;
    startBox.score = 0;
    queue.insert(startBox);
    boxes[start] = startBox;

    bool ok = false;
    size_t iter = 0;
    while (!ok && queue.size() && (maxIter==0 || (iter++) < maxIter)) {
        auto it = queue.begin();
        auto currentBox = *it;
        /*
           for (auto e : queue) {
           std::cout << "~> " << e.pos.x << ", " << e.pos.y << " / " << e.score << std::endl;
           }
           std::cout << "* Picking [" << currentBox.pos.x << " , " << currentBox.pos.y << "] with score of [" 
           << currentBox.score << "]" << std::endl;
           */
        queue.erase(it);

        for (int dx=-1; dx<=1; dx++) {
            for (int dy=-1; dy<=1; dy++) {
                if (dx == 0 && dy == 0) continue;
                Pos pos = currentBox.pos;
                pos.x += dx;
                pos.y += dy;

                // The box is our goal
                if (pos.x == goal.x && pos.y == goal.y) {
                    ok = true;
                }

                // The box is unknown 
                if (!boxes.count(pos)) {
                    Box box;
                    box.pos = pos;
                    box.parent = currentBox.pos;

                    if (reachable(pos)) {
                        // Score to reach this box
                        box.reachScore = currentBox.reachScore;
                        if (dx == 0 || dy == 0) {
                            box.reachScore += 1;
                        } else {
                            box.reachScore += sqrt(2);
                        }
                        // The score of the box is the score to reach it plus the
                        // distance to reach the goal
                        box.score = box.reachScore + goal.dist(box.pos);
                        /*
                           std::cout << "Inserting [" << pos.x << ", " << pos.y 
                           << "] with score [" << box.score << "]" << std::endl;
                           */
                        queue.insert(box);
                    } else {
                        // The box can't be reached
                        box.score = -1;
                    }

                    // Registering the box
                    boxes[pos] = box;
                }
            }
        }
    }

    // Construct the path
    std::vector<Pos> result;
    if (ok) {
        auto box = boxes[goal];
        if (score != NULL) {
            *score = box.score;
        }

        do {
            result.push_back(box.pos);
            box = boxes[box.parent];
        } while (box.score != 0);
        result.push_back(start);

        std::reverse(result.begin(), result.end());
    }
    return result;
}

static inline Point posToPoint(AStar::Pos pos, double step)
{
    return Point(pos.x*step, pos.y*step);
}

static inline AStar::Pos pointToPos(Point point, double step)
{
    return AStar::Pos(round(point.x/step), round(point.y/step));
}

std::vector<Point> AStar::solveCont(
        Point start,
        Point goal,
        std::function<bool(Point)> reachable,
        double step,
        size_t maxIter,
        double *score
        )
{
    auto resultPos = AStar::solve(
            pointToPos(start, step),
            pointToPos(goal, step),
            [&reachable, &step](AStar::Pos pos) -> bool {
                return reachable(posToPoint(pos, step));
            },
            maxIter,
            score);

    std::vector<Point> result;
    for (auto &pos : resultPos) {
        result.push_back(posToPoint(pos, step));
    }

    return result;
}


