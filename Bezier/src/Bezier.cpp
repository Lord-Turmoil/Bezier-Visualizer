#include <vector>
#include "../inc/Bezier.h"

#include <assert.h>

#include "../../Dungine/inc/game/AbstractComponent.h"


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


int InterpolateWithBezierCurve(const Point& p0, const Point& p1, const Point& v0, const Point& v1, double c0,
                               double c1, std::vector<Point>* points)
{
    if (!points)
    {
        return 0;
    }

    double _p0[2] = { p0.x, p0.y };
    double _p1[2] = { p1.x, p1.y };
    double _v0[2] = { v0.x, v0.y };
    double _v1[2] = { v1.x, v1.y };

    double control_points[MAX_CONTROL_POINTS][2];

    int ret = InterpPTC(_p0, _p1, _v0, _v1, c0, c1, control_points);
    if (ret > 0)
    {
        for (int i = 0; i <= ret; i++)
        {
            points->emplace_back(control_points[i][0], control_points[i][1]);
        }
    }
    return ret;
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
        if (t < ZERO)
        {
            return NAN;
        }

        k = k / t;
        return k * 100.0;
    }


    /// \brief Interpolate one side.
    /// \param norm Direction to the point to be interpolated.
    /// \param step Current Step.
    /// \param target Target curvature value.
    /// \param p1 Point to calculate curvature.
    /// \param p2 Point to calculate curvature.
    /// \param p3 Point to calculate curvature.
    /// \param begin Whether is the begin of the curve. \see CalcCurvature
    /// \return
    ///     -1: failed
    ///      0: already satisfied
    ///      1: new step found
    static int InterpPTCImpl(
        double p1[2],
        double p2[2],
        double p3[2],
        double norm[2],
        double* step,
        double target,
        int begin
    );


    int InterpPTC(
        double p1[2],
        double p2[2],
        double v1[2],
        double v2[2],
        double c1,
        double c2,
        double(*control_points)[2])
    {
        // Calculate step norm.
        const double v1Mod = sqrt(v1[0] * v1[0] + v1[1] * v1[1]);
        const double v2Mod = sqrt(v2[0] * v2[0] + v2[1] * v2[1]);
        if (v1Mod < ZERO || v2Mod < ZERO)
        {
            return ERR_NORM_TOO_SMALL;
        }
        double v1Norm[2] = { v1[0] / v1Mod, v1[1] / v1Mod };
        // To make interpolate easier, we invert this vector.
        double v2Norm[2] = { -v2[0] / v2Mod, -v2[1] / v2Mod };

        double left[2] = { p1[0], p1[1] };
        double right[2] = { p2[0], p2[1] };
        double stepLeft = 0.0;
        double stepRight = 0.0;
        for (int i = 0; i < MAX_ITERATION; i++)
        {
            const int retRight = InterpPTCImpl(p1, left, right, v1Norm, &stepLeft, c1, 1);
            if (retRight < 0)
            {
                return retRight;
            }
            left[0] = p1[0] + stepLeft * v1Norm[0];
            left[1] = p1[1] + stepLeft * v1Norm[1];

            const int retLeft = InterpPTCImpl(left, right, p2, v2Norm, &stepRight, c2, 0);
            if (retLeft < 0)
            {
                return retLeft;
            }
            right[0] = p2[0] + stepRight * v2Norm[0];
            right[1] = p2[1] + stepRight * v2Norm[1];

            if (retLeft == 0 && retRight == 0)
            {
                // Success
                control_points[0][0] = p1[0];
                control_points[0][1] = p1[1];
                control_points[1][0] = left[0];
                control_points[1][1] = left[1];
                control_points[2][0] = right[0];
                control_points[2][1] = right[1];
                control_points[3][0] = p2[0];
                control_points[3][1] = p2[1];
                return 3;
            }
        }

        // We still show it if failure accepted.
#ifdef ACCEPT_FAILURE
        control_points[0][0] = p1[0];
        control_points[0][1] = p1[1];
        control_points[1][0] = left[0];
        control_points[1][1] = left[1];
        control_points[2][0] = right[0];
        control_points[2][1] = right[1];
        control_points[3][0] = p2[0];
        control_points[3][1] = p2[1];
        return 3;
#endif

        return ERR_OUT_OF_ITERATION;
    }


    static int IsSatisfied(double c, double target)
    {
        return !isnan(c) && fabs(c - target) < EPS;
    }


    static int InterpPTCImpl(
        double p1[2],
        double p2[2],
        double p3[2],
        double norm[2],
        double* step,
        double target,
        int begin
    )
    {
        assert(step);

        // Check initial condition.
        double c = CalcCurvature(p1, p2, p3, begin);
        if (IsSatisfied(c, target))
        {
            return 0;
        }

        double res[2];
        const double* const base = begin ? p1 : p3;
        // If c is too big, make point further.
        if (isnan(c) || c - target > EPS)
        {
            double delta = EPS;
            *step += delta;
            delta *= 2.0;
            while (isnan(c) || c > target)
            {
                res[0] = base[0] + *step * norm[0];
                res[1] = base[1] + *step * norm[1];
                c = CalcCurvature(p1, res, p3, begin);
                *step += delta;
            }
            if (!isnan(c) && fabs(c - target) < EPS)
            {
                return 1;
            }
        }

        double left = 0;
        double right = *step;
        // Now, c is bigger, use dichotomy to find a proper value
        // between lastDelta and delta.
        while (right - left > ZERO)
        {
            const double mid = left + (right - left) * 0.5;
            res[0] = base[0] + mid * norm[0];
            res[1] = base[1] + mid * norm[1];

            c = CalcCurvature(p1, res, p3, begin);
            if (IsSatisfied(c, target))
            {
                *step = mid;
                return 1;
            }
            if (c < target)
            {
                right = mid;
            }
            else
            {
                left = mid;
            }
        }

        *step = left;

        return ERR_DICHOTOMY_FAILED;
    }
}
