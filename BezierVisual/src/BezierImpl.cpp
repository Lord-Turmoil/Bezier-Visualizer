#include "BezierImpl.h"

static void DrawControlPoint(double x, double y, COLORREF color);
static void DrawCurvePoint(double x, double y);
static void DrawLine(const Point& from, const Point& to, COLORREF color);
static void DrawBezierCurve(const std::vector<Point>& points, double step);
static void DrawCoordinates(const std::vector<Point>& points);
static void DrawBezierControlLines(const std::vector<std::vector<Point>>& lines);


BezierImpl::BezierImpl() : _draggingPoint(nullptr), _step(1.0), _showCoordinate(false), _showCurvature(false)
{

}


void BezierImpl::AddControlPoint(const Point& point)
{
    _controlPoints.push_back(point);
}


void BezierImpl::RemoveControlPoint(Point* point)
{
    if (point)
    {
        for (auto it = _controlPoints.begin(); it != _controlPoints.end(); ++it)
        {
            if (&(*it) == point)
            {
                _controlPoints.erase(it);
                break;
            }
        }
    }
}


Point* BezierImpl::FindCandidatePoint(const Point& point)
{
    double minDistance = 100.0;
    Point* draggingPoint = nullptr;

    for (auto& p : _controlPoints)
    {
        const double dx = p.x - point.x;
        const double dy = p.y - point.y;
        const double dist = dx * dx + dy * dy;
        if (dist < minDistance)
        {
            minDistance = dist;
            draggingPoint = &p;
        }
    }

    return draggingPoint;
}


void BezierImpl::DragPoint(const Point& point) const
{
    if (_draggingPoint)
    {
        _draggingPoint->x = point.x;
        _draggingPoint->y = point.y;
    }
}


void BezierImpl::Render()
{
    cleardevice();

    InitControlLines(&_controlLines, &_controlPoints, _step);
    DrawBezierControlLines(_controlLines);

    DrawBezierCurve(_controlPoints, _step);
    if (_draggingPoint)
    {
        setlinecolor(0x2257FF);
        circle(static_cast<int>(_draggingPoint->x + 0.5), static_cast<int>(_draggingPoint->y + 0.5), 10);
    }

    if (_showCoordinate)
    {
        DrawCoordinates(_controlPoints);
    }

    if (_showCurvature)
    {
        // DrawCurvature(_controlPoints);
    }
}


double BezierImpl::FindAttachX(double x) const
{
    static const double minDistance = 30.0;
    double ret = x;

    for (auto& point : _controlPoints)
    {
        if (_draggingPoint && &point == _draggingPoint)
        {
            continue;
        }

        if (fabs(point.x - x) < minDistance)
        {
            ret = point.x;
        }
    }

    return ret;
}


double BezierImpl::FindAttachY(double y) const
{
    static const double minDistance = 30.0;
    double ret = y;

    for (auto& point : _controlPoints)
    {
        if (_draggingPoint && &point == _draggingPoint)
        {
            continue;
        }

        if (fabs(point.y - y) < minDistance)
        {
            ret = point.y;
        }
    }

    return ret;
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
    line(static_cast<int>(from.x + 0.5), static_cast<int>(from.y + 0.5), static_cast<int>(to.x + 0.5),
         static_cast<int>(to.y + 0.5));
}


static void DrawBezierCurve(const std::vector<Point>& points, double step)
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


static void DrawCoordinates(const std::vector<Point>& points)
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


static void DrawBezierControlLines(const std::vector<std::vector<Point>>& lines)
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
