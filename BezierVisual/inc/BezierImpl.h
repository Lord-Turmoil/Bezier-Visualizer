#pragma once

#ifndef _BEZIERVISUAL_BEZIER_IMPL_H_
#define _BEZIERVISUAL_BEZIER_IMPL_H_

#include "Macros.h"
#include <vector>


class BezierImpl
{
public:
    BezierImpl();

    Point* AddControlPoint(const Point& point);
    Point* AddControlPoint(double x, double y) { return AddControlPoint(Point(x, y)); }
    void ClearControlPoints() { _controlPoints.clear(); }
    void RemoveControlPoint(Point* point);

    void SetStep(double step) { _step = step; }

    /// \brief Find the nearest point within range.
    /// \param point The actual point
    /// \return The nearest point within range
    Point* FindCandidatePoint(const Point& point);
    Point* FindCandidatePoint(double x, double y) { return FindCandidatePoint(Point(x, y)); }
    void SetDraggingPoint(Point* point) { _draggingPoint = point; }
    void SetDraggingPoint(double x, double y) { SetDraggingPoint(FindCandidatePoint(x, y)); }

    void DragPoint(const Point& point) const;
    void DragPoint(double x, double y) const { DragPoint(Point(x, y)); }

    void ToggleShowCoordinate() { _showCoordinate = !_showCoordinate; }
    void ToggleShowCurvature() { _showCurvature = !_showCurvature; }

    double FindAttachX(double x) const;
    double FindAttachY(double y) const;

    void Render();

    const std::vector<Point>& GetControlPoints() const { return _controlPoints; }

private:
    std::vector<Point> _controlPoints;
    std::vector<std::vector<Point>> _controlLines;
    Point* _draggingPoint;

    double _step;

    bool _showCoordinate;
    bool _showCurvature;
};

#endif
