#pragma once

#include <functional>
#include <geometry/Circle.hpp>
#include <geometry/Point.hpp>

class ObstacleAvoider
{
    public:
        void addObstacle(Point center, double radius);

        std::vector<Point> findPath(
                Point start, 
                Point goal, 
                double accuracy=20, 
                double *score = NULL,
                std::function<bool(Point)> filter = [](Point) { return true; }
                );

    protected:
        std::vector<Circle> obstacles;
};
