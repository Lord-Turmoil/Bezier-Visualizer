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
    int n = prev.size() - 1;

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

static void DrawControlPoint(double x, double y, COLORREF color)
{
    // Round floating point coordinates to integer coordinates.
    setfillcolor(color);
    solidcircle(static_cast<int>(x + 0.5), static_cast<int>(y + 0.5), 5);
}

static void DrawCurvePoint(double x, double y)
{
    setfillcolor(0xFFBF00);
    solidcircle(static_cast<int>(x + 0.5), static_cast<int>(y + 0.5), 3);
}

static void DrawLine(const Point& from, const Point& to, COLORREF color)
{
    setlinecolor(color);
    line((int)(from.x + 0.5), (int)(from.y + 0.5), (int)(to.x + 0.5), (int)(to.y + 0.5));
}

void DrawBezierCurve(const std::vector<Point>& points, double step)
{
    int n = static_cast<int>(points.size()) - 1;
    if (n > 0)
    {
        for (double t = 0.0; t <= step; t += 0.001)
        {
            Point point;
            for (int k = 0; k <= n; k++)
            {
                point = point + Combination(n, k) * pow(t, k) * pow(1 - t, n - k) * points[k];
            }
            DrawCurvePoint(point.x, point.y);
        }
    }

    for (const auto& point : points)
    {
        DrawControlPoint(point.x, point.y, 0x228B22);
    }
}

void DrawBezierControlLines(const std::vector<std::vector<Point>>& lines)
{
    static const COLORREF COLORS[] = {
        RGB(244, 67, 54), RGB(156, 39, 176), RGB(63, 81, 181), RGB(3, 169, 244),
        RGB(0, 150, 136), RGB(139, 195, 74), RGB(255, 235, 59), RGB(255, 152, 0)
    };

    int i = 0;
    for (auto& line : lines)
    {
        int n = line.size();
        COLORREF color = COLORS[(i++) % 8];
        for (auto& point : line)
        {
            DrawControlPoint(point.x, point.y, color);
        }
        setlinecolor(color);
        for (int j = 1; j < n; j++)
        {
            DrawLine(line[j - 1], line[j], color);
        }
    }
}

