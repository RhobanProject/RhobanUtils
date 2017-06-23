#pragma once

#include <geometry/Circle.hpp>
#include <geometry/Point.hpp>

class ObstacleAvoider
{
    public:
        void addObstacle(Point center, double radius);

        std::vector<Point> findPath(Point start, Point goal, int accuracy = 16, double *score = NULL);

        double margin = 25;

    protected:
        std::vector<Circle> obstacles;
};
