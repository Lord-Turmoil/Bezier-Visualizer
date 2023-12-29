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

    if (c0)
    {
        double _p1[2] = { points[0].x, points[0].y };
        double _p2[2] = { points[1].x, points[1].y };
        double _p3[2] = { points[2].x, points[2].y };
        *c0 = CalcCurvature(_p1, _p2, _p3, 1);
    }
    if (c1)
    {
        double _p1[2] = { points[n - 3].x, points[n - 3].y };
        double _p2[2] = { points[n - 2].x, points[n - 2].y };
        double _p3[2] = { points[n - 1].x, points[n - 1].y };
        *c1 = CalcCurvature(_p1, _p2, _p3, 0);
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
    double CalcCurvature(double p1[2], double p2[2], double p3[2], int begin)
    {
        double v1[2] = { p2[0] - p1[0], p2[1] - p1[1] };
        double v2[2] = { p3[0] - p2[0], p3[1] - p2[1] };
        double cross[2] = { v1[0] * v2[1] - v1[1] * v2[0], 0 };

        double k = sqrt(cross[0] * cross[0] + cross[1] * cross[1]);
        double t = begin ? sqrt(v1[0] * v1[0] + v1[1] * v1[1]) : sqrt(v2[0] * v2[0] + v2[1] * v2[1]);
        t = t * t * t;
        k = k / t;
        return k * 100.0;
    }

    int InterpPTC(
        double p1[2],
        double p2[2],
        double v1[2],
        double v2[2],
        double c1,
        double c2,
        double(*control_points)[2])
    {
        // Interpolate with 4 points Bezier curve.
        // p1, p2 are the start and end points.

        return -1;
    }
}
