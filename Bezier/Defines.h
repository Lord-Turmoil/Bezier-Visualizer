#pragma once

#ifndef _BEZIER_DEFINES_H_
#define _BEZIER_DEFINES_H_


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
        return Point({ x + p.x, y + p.y });
    }


    friend Point operator*(Point lhs, double rhs)
    {
        return Point({ lhs.x * rhs, lhs.y * rhs });
    }


    friend Point operator*(double lhs, Point rhs)
    {
        return Point({ lhs * rhs.x, lhs * rhs.y });
    }
};

#endif // _BEZIER_DEFINES_H_
