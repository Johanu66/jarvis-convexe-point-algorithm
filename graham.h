#ifndef GRAHAM_H
#define GRAHAM_H

#include <iostream>
#include <vector>
#include <algorithm>
#include <stack>
#include "point.h"

using namespace std;

class Graham{
  private:
    Point _pivot_point;
    vector<Point> _points;
    vector<Point> _convex_hull_points;
  public:
    Graham(vector<Point> points);
    vector<Point> getConvexHullPoints() const;
    void convexHull();
};
bool comparePolarAngle(const Point& p1, const Point& p2);

#endif