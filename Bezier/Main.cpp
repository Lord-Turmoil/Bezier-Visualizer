#include <cmath>
#include <cstdio>
#include <easyx.h>
#include <vector>

#include "Bezier.h"
#include "Defines.h"
#include "Interface.h"

const double STICK_DISTANCE = 30.0;

static bool isRunning;
static std::vector<Point> controlPoints;
static std::vector<std::vector<Point>> controlLines;
static Point* draggingPoint;
static bool isSliding;
static double step = 1.0;
static bool showCoordinate = false;

/// \brief Process user input.
void ProcessInput();

/// \brief Process slide.
bool ProcessSlide(int x, int y);

/// \brief Find a control point to be selected, and set it as
/// draggingPoint. If not found, draggingPoint will be nullptr.
/// \param x Mouse x.
/// \param y Mouse y.
/// \return Return whether a control point can be selected.
bool FindCandidateControlPoint(double x, double y);

/// \brief Try to stick x to an existing control point.
/// \param x The x coordinate to be stuck.
/// \return The x coordinate of the nearest control point. If no stick
/// found, x will be returned.
double FindStickX(double x);

/// \brief Try to stick x to an existing control point.
/// \param y The y coordinate to be stuck.
/// \return The y coordinate of the nearest control point. If no stick
/// found, y will be returned.
double FindStickY(double y);

/// \brief Create a new control point at the specified coordinate.
/// \param x X coordinate of the control point.
/// \param y Y coordinate of the control point.
void CreateControlPoint(double x, double y);

/// \brief Clear all control points.
void ClearControlPoints();


int APIENTRY WinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPSTR lpCmdLine,
    _In_ int nShowCmd)
{
    initgraph(WIDTH, HEIGHT);
    setbkcolor(WHITE);
    setbkmode(TRANSPARENT);
    BeginBatchDraw();

    isRunning = true;
    while (isRunning)
    {
        // Get user input.
        ProcessInput();

        InitControlLines(&controlLines, &controlPoints, step);

        // Draw.
        cleardevice();
        DrawBezierCurve(controlPoints, step);
        DrawBezierControlLines(controlLines);
        if (showCoordinate)
        {
            DrawCoordinates(controlPoints);
        }

        DrawControls(step, draggingPoint);
        DrawHelp();

        FlushBatchDraw();
        Sleep(18);
    }

    return 0;
}


void ProcessInput()
{
    ExMessage msg;
    while (peekmessage(&msg, EX_MOUSE | EX_KEY))
    {
        if (msg.message == WM_KEYDOWN)
        {
            switch (msg.vkcode)
            {
            case VK_ESCAPE:
                isRunning = false;
                break;
            case 'C':
                showCoordinate = !showCoordinate;
                break;
            default:
                break;
            }
        }
        else if (msg.message == WM_LBUTTONDOWN)
        {
            // Find the nearest control point.
            if (!FindCandidateControlPoint(msg.x, msg.y))
            {
                if (!ProcessSlide(msg.x, msg.y))
                {
                    CreateControlPoint(msg.x, msg.y);
                    draggingPoint = &controlPoints.back();
                }
            }
        }
        else if (msg.message == WM_LBUTTONUP)
        {
            if (draggingPoint)
            {
                draggingPoint = nullptr;
            }
            isSliding = false;
        }
        else if (msg.message == WM_MOUSEMOVE && msg.lbutton)
        {
            // Drag existing point.
            if (draggingPoint)
            {
                if (msg.shift)
                {
                    draggingPoint->x = FindStickX(msg.x);
                    draggingPoint->y = FindStickY(msg.y);
                }
                else
                {
                    draggingPoint->x = msg.x;
                    draggingPoint->y = msg.y;
                }
            }

            if (isSliding)
            {
                ProcessSlide(msg.x, msg.y);
            }
        }
        else if (msg.message == WM_RBUTTONUP)
        {
            // Clear all control points.
            ClearControlPoints();
        }
    }
}


bool ProcessSlide(int x, int y)
{
    if (isSliding)
    {
        step = (x - SLIDE_X) / static_cast<double>(SLIDE_WIDTH);
        if (step < 0.0)
        {
            step = 0.0;
        }
        else if (step > 1.0)
        {
            step = 1.0;
        }
        return true;
    }

    if (x >= SLIDE_X - 5 && x <= SLIDE_X + SLIDE_WIDTH + 5 &&
        y >= SLIDE_Y - 8 && y <= SLIDE_Y + SLIDE_HEIGHT + 8)
    {
        isSliding = true;
    }

    return isSliding;
}


bool FindCandidateControlPoint(double x, double y)
{
    double minDistance = 100.0;
    draggingPoint = nullptr;

    for (auto& p : controlPoints)
    {
        double dx = p.x - x;
        double dy = p.y - y;
        double dist = dx * dx + dy * dy;
        if (dist < minDistance)
        {
            minDistance = dist;
            draggingPoint = &p;
        }
    }

    return draggingPoint;
}


double FindStickX(double x)
{
    double minDistance = STICK_DISTANCE;
    double ret = x;

    for (auto& point : controlPoints)
    {
        if (draggingPoint && &point == draggingPoint)
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


double FindStickY(double y)
{
    double minDistance = STICK_DISTANCE;
    double ret = y;

    for (auto& point : controlPoints)
    {
        if (draggingPoint && &point == draggingPoint)
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


void CreateControlPoint(double x, double y)
{
    controlPoints.emplace_back(x, y);
}


void ClearControlPoints()
{
    controlPoints.clear();
    draggingPoint = nullptr;
}
