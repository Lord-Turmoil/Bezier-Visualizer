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
