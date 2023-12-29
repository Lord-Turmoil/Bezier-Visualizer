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


    /// \brief This is not strict cross product. Only for 2D vector.
    /// \param p rhs
    /// \return Cross
    Point Cross(const Point& p) const
    {
        return { x * p.y - y * p.x, 0 };
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

/// \brief C++ Wrapping for \see InterPTC.
int InterpolateWithBezierCurve(
    const Point& p0,
    const Point& p1,
    const Point& v0,
    const Point& v1,
    double c0,
    double c1, std::vector<Point>* points);

extern "C"
{
    const int MAX_CONTROL_POINTS = 10;
    const double EPS = 1e-2;
    const double ZERO = 1e-6;
    const int MAX_ITERATION = 1000000;

    const int ERR_ILLEGAL_ARGUMENT = -100;
    const int ERR_NORM_TOO_SMALL = -101;
    const int ERR_OUT_OF_ITERATION = -102;

    const int ERR_FAILED_TO_INTERPOLATE = -111;
    const int ERR_DICHOTOMY_FAILED = -112;

    double CalcCurvature(double p1[2], double p2[2], double p3[2], int begin);

    /// \brief Interpolate points with Bezier curve.
    /// \param p1 Starting point.
    /// \param p2 Ending point.
    /// \param v1 Starting tangent vector.
    /// \param v2 Ending tangent vector.
    /// \param c1 Curvature at the starting point.
    /// \param c2 Curvature at the ending point.
    /// \param control_points Control points of Bezier curve.
    /// \return The power of Bezier curve. Negative to indicate error.
    int InterpPTC(
        double p1[2],
        double p2[2],
        double v1[2],
        double v2[2],
        double c1,
        double c2,
        double(*control_points)[2]);
}


#ifdef _DEBUG
#pragma comment(lib, "lib/Debug/bezier_b.lib")
#define ACCEPT_FAILURE
#else
#pragma comment(lib, "lib/Release/bezier.lib")
#endif

#endif // _BEZIER_UTILS_H_
