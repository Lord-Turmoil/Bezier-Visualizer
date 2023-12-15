#include <cmath>
#include <cstdio>
#include <easyx.h>
#include <vector>

#include "Bezier.h"
#include "Defines.h"

const int WIDTH = 800;
const int HEIGHT = 600;
const double STICK_DISTANCE = 30.0;

static const int SLIDE_X = 30;
static const int SLIDE_Y = HEIGHT - 40;
static const int SLIDE_WIDTH = WIDTH - 180;
static const int SLIDE_HEIGHT = 10;

static bool isRunning;
static std::vector<Point> controlPoints;
static std::vector<std::vector<Point>> controlLines;
static Point* draggingPoint;
static bool isSliding;
static double step = 1.0;

/// \brief Process user input.
void ProcessInput();

/// \brief Process slide.
bool ProcessSlide(int x, int y);

/// \brief Draw slide control.
void DrawControls();

/// \brief Draw help text.
void DrawHelp();

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

        DrawControls();
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
        if (msg.message == WM_KEYDOWN && msg.vkcode == VK_ESCAPE)
        {
            isRunning = false;
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


void DrawControls()
{
    static wchar_t buffer[64];

    setfillcolor(0xBDBDBD);
    solidrectangle(SLIDE_X, SLIDE_Y, SLIDE_X + SLIDE_WIDTH, SLIDE_Y + SLIDE_HEIGHT);

    setfillcolor(0x0098FF);
    solidrectangle(SLIDE_X, SLIDE_Y, SLIDE_X + static_cast<int>(SLIDE_WIDTH * step), SLIDE_Y + SLIDE_HEIGHT);

    setlinestyle(PS_SOLID, 2);
    setlinecolor(0x9E9E9E);
    setfillcolor(0xE0E0E0);
    fillrectangle(SLIDE_X + static_cast<int>(SLIDE_WIDTH * step) - 10, SLIDE_Y - 10, SLIDE_X + static_cast<int>(SLIDE_WIDTH * step) + 10, SLIDE_Y + SLIDE_HEIGHT + 10);

    settextstyle(24, 0, L"Consolas");
    swprintf_s(buffer, L"t = %.2f", step);
    outtextxy(SLIDE_X + SLIDE_WIDTH + 20, SLIDE_Y - 8, buffer);
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
    outtextxy(10, y += lineHeight, L"按 ESC 退出");
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
