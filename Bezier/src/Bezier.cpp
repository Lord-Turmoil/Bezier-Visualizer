#include <vector>
#include "../inc/Bezier.h"


int Combination(int n, int k)
{
    static std::vector<std::vector<int>> triangle(1, std::vector<int>(1, 1));
    static int maxN = 1;

    while (n >= maxN)
    {
        triangle.emplace_back(maxN + 1, 1);
        for (int i = 1; i < maxN; i++)
            triangle[maxN][i] = triangle[maxN - 1][i - 1] + triangle[maxN - 1][i];
        triangle[maxN].push_back(1);
        maxN++;
    }

    return triangle[n][k];
}


void InitControlLines(std::vector<std::vector<Point>>* lines, const std::vector<Point>* points, double step)
{
    if (points)
    {
        lines->clear();
        lines->emplace_back(*points);
    }

    std::vector<Point>& prev = lines->back();
    std::vector<Point> next;
    int n = static_cast<int>(prev.size()) - 1;

    if (n <= 0)
    {
        return;
    }

    for (int i = 0; i < n; i++)
    {
        Point point = (1 - step) * prev[i] + step * prev[i + 1];
        next.push_back(point);
    }
    lines->emplace_back(next);
    InitControlLines(lines, nullptr, step);
}


bool CalculateCurvature(const std::vector<Point>& points, double* c0, double* c1)
{
    int n = static_cast<int>(points.size());
    if (n < 2)
    {
        return false;
    }

    if (n == 2)
    {
        if (c0)
        {
            *c0 = 0.0;
        }
        if (c1)
        {
            *c1 = 0.0;
        }
        return true;
    }

    // P[0...n]
    n--;


    if (c0)
    {
        double k0 = static_cast<double>(n - 1) / static_cast<double>(n);
        double t0 = (points[1] - points[0]).Cross(points[2] - points[1]).Mod();
        double t1 = (points[1] - points[0]).Mod();
        t1 = t1 * t1 * t1;
        k0 = k0 * t0 / t1;
        *c0 = k0 * 100;
    }
    if (c1)
    {
        double k1 = static_cast<double>(n - 1) / static_cast<double>(n);
        double t0 = (points[n - 1] - points[n - 2]).Cross(points[n] - points[n - 1]).Mod();
        double t1 = (points[n] - points[n - 1]).Mod();
        t1 = t1 * t1 * t1;
        k1 = k1 * t0 / t1;
        *c1 = k1 * 100;
    }

    return true;
}


bool InterpolateWithBezierCurve(const Point& p0, const Point& p1, const Point& v0, const Point& v1, double c0,
                                double c1, std::vector<Point>* points)
{
    if (!points)
    {
        return false;
    }

    double _p0[2] = { p0.x, p0.y };
    double _p1[2] = { p1.x, p1.y };
    double _v0[2] = { v0.x, v0.y };
    double _v1[2] = { v1.x, v1.y };

    double control_points[MAX_CONTROL_POINTS][2];

    int ret = InterpPTC(_p0, _p1, _v0, _v1, c0, c1, control_points);
    if (ret < 0)
    {
        return false;
    }

    for (int i = 0; i <= ret; i++)
    {
        points->emplace_back(control_points[i][0], control_points[i][1]);
    }

    return true;
}


extern "C"
{
    int InterpPTC(double p1[2], double p2[2], double v1[2], double v2[2], double c1, double c2,
                  double(*control_points)[2])
    {
        double a = 2 * p1[0] - 2 * p2[0] + c1 * v1[0] + c2 * v2[0];
        double b = -3 * p1[0] + 3 * p2[0] - 2 * c1 * v1[0] - c2 * v2[0];
        double c = c1 * v1[0];
        double d = p1[0];

        double e = 2 * p1[1] - 2 * p2[1] + c1 * v1[1] + c2 * v2[1];
        double f = -3 * p1[1] + 3 * p2[1] - 2 * c1 * v1[1] - c2 * v2[1];
        double g = c1 * v1[1];
        double h = p1[1];

        double t = 0.0;
        double dt = 0.01;
        int i = 0;
        while (t < 1.0)
        {
            double x = a * t * t * t + b * t * t + c * t + d;
            double y = e * t * t * t + f * t * t + g * t + h;
            control_points[i][0] = x;
            control_points[i][1] = y;
            t += dt;
            i++;
            if (i >= MAX_CONTROL_POINTS)
            {
                // Failed!
                return -1;
            }
        }

        return i - 1;
    }
}
