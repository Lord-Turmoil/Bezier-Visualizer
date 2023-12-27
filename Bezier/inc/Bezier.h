#pragma once

#ifndef _BEZIER_H_
#define _BEZIER_H_

#include <vector>
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

/// \brief Calculate the combination number of n and k.
/// \param n The number of elements.
/// \param k The number of elements to be chosen.
/// \return The combination number of n and k.
int Combination(int n, int k);

/// \brief Initialize Bezier curve control lines.
/// \param lines Control lines of Bezier curve.
/// \param step Range in [0, 1], 0 means nothing, 1 means complete.
void InitControlLines(std::vector<std::vector<Point>>* lines, const std::vector<Point>* points, double step);

/// \brief Calculate curvature at the starting point and the ending point of Bezier curve.
/// \param points Bezier curve control points.
/// \param c0 Out curvature at the starting point.
/// \param c1 Out Curvature at the ending point.
/// \return Whether curvature is valid.
bool CalculateCurvature(const std::vector<Point>& points, double* c0, double* c1);

#ifdef _DEBUG
#pragma comment(lib, "lib/Debug/bezier_b.lib")
#else
#pragma comment(lib, "lib/Release/bezier.lib")
#endif

#endif // _BEZIER_UTILS_H_
