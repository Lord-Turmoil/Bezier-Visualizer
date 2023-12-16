#include <easyx.h>
#include <vector>
#include "Bezier.h"


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
