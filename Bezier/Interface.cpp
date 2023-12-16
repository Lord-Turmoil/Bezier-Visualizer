#include <easyx.h>
#include "Bezier.h"
#include "Interface.h"


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
    line(static_cast<int>(from.x + 0.5), static_cast<int>(from.y + 0.5), static_cast<int>(to.x + 0.5),
         static_cast<int>(to.y + 0.5));
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


void DrawCoordinates(const std::vector<Point>& points)
{
    static wchar_t buffer[64];
    settextstyle(18, 0, L"Consolas");
    settextcolor(BLACK);
    for (auto& point : points)
    {
        swprintf_s(buffer, L"(%.2f, %.2f)", point.x, point.y);
        outtextxy(static_cast<int>(point.x + 0.5), static_cast<int>(point.y + 0.5), buffer);
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
        int n = static_cast<int>(line.size());
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


void DrawControls(double step, Point* draggingPoint)
{
    static wchar_t buffer[64];

    setfillcolor(0xBDBDBD);
    solidrectangle(SLIDE_X, SLIDE_Y, SLIDE_X + SLIDE_WIDTH, SLIDE_Y + SLIDE_HEIGHT);

    setfillcolor(0x0098FF);
    solidrectangle(SLIDE_X, SLIDE_Y, SLIDE_X + static_cast<int>(SLIDE_WIDTH * step), SLIDE_Y + SLIDE_HEIGHT);

    setlinestyle(PS_SOLID, 2);
    setlinecolor(0x9E9E9E);
    setfillcolor(0xE0E0E0);
    fillrectangle(SLIDE_X + static_cast<int>(SLIDE_WIDTH * step) - 10, SLIDE_Y - 10,
                  SLIDE_X + static_cast<int>(SLIDE_WIDTH * step) + 10, SLIDE_Y + SLIDE_HEIGHT + 10);

    settextstyle(24, 0, L"Consolas");
    settextcolor(BLACK);
    swprintf_s(buffer, L"t = %.2f", step);
    outtextxy(SLIDE_X + SLIDE_WIDTH + 20, SLIDE_Y - 8, buffer);

    if (draggingPoint)
    {
        setlinecolor(0x2257FF);
        circle(static_cast<int>(draggingPoint->x + 0.5), static_cast<int>(draggingPoint->y + 0.5), 10);
    }
}


void DrawHelp()
{
    settextstyle(24, 0, L"Microsoft YaHei UI");
    settextcolor(BLACK);

    int y = 10;
    int lineHeight = 30;
    outtextxy(10, y, L"鼠标左键新建控制点");
    outtextxy(10, y += lineHeight, L"鼠标左键按下拖动控制点");
    outtextxy(10, y += lineHeight, L"鼠标右键清空屏幕");
    outtextxy(10, y += lineHeight, L"按 Shift 开启吸附");
    outtextxy(10, y += lineHeight, L"按 C 切换坐标显示");
    outtextxy(10, y += lineHeight, L"按 ESC 退出");
}


void DrawCopyright()
{
    settextstyle(18, 0, L"Consolas");
    settextcolor(0x5F5F5F);
    outtextxy(WIDTH - 280, 10, L"Copyright (C) Tony's Studio 2023");
}
