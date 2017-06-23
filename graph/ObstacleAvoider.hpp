#pragma once

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
                double *score = NULL
                );

    protected:
        std::vector<Circle> obstacles;
};
