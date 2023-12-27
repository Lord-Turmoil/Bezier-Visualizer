#pragma once

#ifndef _BEZIER_DEFINES_H_
#define _BEZIER_DEFINES_H_

#include <cmath>

struct Point
{
    double x;
    double y;


    Point() : x(0.0), y(0.0)
    {
    }


    Point(double _x, double _y) : x(_x), y(_y)
    {
    }


    Point operator+(const Point& p) const
    {
        return { x + p.x, y + p.y };
    }

    Point operator-(const Point& p) const
    {
        return { x - p.x, y - p.y };
    }

    friend Point operator*(Point lhs, double rhs)
    {
        return { lhs.x * rhs, lhs.y * rhs };
    }


    friend Point operator*(double lhs, Point rhs)
    {
        return { lhs * rhs.x, lhs * rhs.y };
    }

    Point Cross(const Point& p) const
    {
        return { x * p.y - y * p.x, y * p.x - x * p.y };
    }

    double Mod() const
    {
        return std::sqrt(x * x + y * y);
    }
};

#endif // _BEZIER_DEFINES_H_
