#include <cmath>
#include <iostream>
#include "Graph.hpp"
#include "Djikstra.hpp"
#include "ObstacleAvoider.hpp"
#include <geometry/Segment.hpp>
#include <geometry/Circle.hpp>

void testDjikstra()
{
    Graph g;

    g.add(1);
    g.add(2);
    g.add(3);
    g.add(4);
    
    g.connect(1, 3, 10);
    g.connect(1, 2, 3);
    g.connect(3, 2, 4.5);
    g.connect(1, 4, 50);
    g.connect(3, 4, 10);

    auto result = Djikstra::findPath(g, 1, 3);

    for (auto &node : result) {
        std::cout << node << std::endl;
    }
}

void testCircleIntersection()
{
    Segment s(Point(1.2, 0), Point(-5, 5));
    Circle c(Point(3, 3), 1.7);

    for (double alpha=0; alpha<1; alpha+=0.01) {
        auto point = s.A+alpha*(s.B-s.A);
        std::cout << point.x << " " << point.y << std::endl;
    }
    
    for (double alpha=0; alpha<360; alpha+=10) {
        auto point = c.getCenter() + Point(c.getRadius(),0).rotation(alpha);
        std::cout << point.x << " " << point.y << std::endl;
    }

    std::cerr << s.intersects(c) << std::endl;
}

void testObstacleAvoider()
{
    ObstacleAvoider oa;
    oa.addObstacle(Point(0, 0), 100);
    oa.addObstacle(Point(-1.5, -150), 100);

    oa.findPath(Point(-200, 0), Point(200, 0));
}

void testCircleTangents()
{
    Circle c(3, 4, 0.5);

    for (double a=0; a<M_PI*2; a+=0.1) {
        auto A = c.getCenter();
        A.x += cos(a)*c.getRadius();
        A.y += sin(a)*c.getRadius();
        auto B = c.getCenter();
        B.x += cos(a+0.1)*c.getRadius();
        B.y += sin(a+0.1)*c.getRadius();
        std::cout << A.x << " " << A.y << std::endl;
        std::cout << B.x << " " << B.y << std::endl;
        std::cout << std::endl << std::endl;
    }

    Point p(2, 10);
    auto tangents = c.tangents(p);

    for (auto &t : tangents) {
        std::cout << p.x << " " << p.y << std::endl;
        std::cout << t.x << " " << t.y << std::endl;
        std::cout << std::endl << std::endl;
    }
}

int main()
{
    // testDjikstra();
    // testCirlceIntersection();
    // testObstacleAvoider();
    testCircleTangents();
}

