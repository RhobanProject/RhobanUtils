#pragma once

#include "Point.hpp"
#include "Circle.hpp"

class Segment
{
    public:
        Segment(Point A, Point B);

        // Returns the alpha of the projection of a point, the alpha
        // is the value so that:
        //
        // point1 + alpha*(point2-point1) = point
        //
        // If it is between 0 and 1, the projected point is on the segment,
        // else it is not
        double projectAlpha(const Point &point);

        // Does a point projects on the segment?
        bool projects(const Point &point);

        // Projects a point on this segment
        Point project(const Point &point);

        // Distance from point to segment
        double distance(const Point &point);

        // Does a circle intersects tis segment?
        bool intersects(const Circle &circle);

        // Get the segment length
        double getLength();

        // The two segments
        Point A, B;
};

