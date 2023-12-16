#pragma once

#ifndef _INTERFACE_H_
#define _INTERFACE_H_

#include <vector>
#include "Defines.h"

const int WIDTH = 800;
const int HEIGHT = 600;

const int SLIDE_X = 30;
const int SLIDE_Y = HEIGHT - 40;
const int SLIDE_WIDTH = WIDTH - 180;
const int SLIDE_HEIGHT = 10;

/// \brief Draw a Bezier curve.
/// \param points Control points of Bezier curve.
/// \param step Range in [0, 1], 0 means nothing, 1 means complete.
void DrawBezierCurve(const std::vector<Point>& points, double step);

/// \brief Draw coordinates for points.
/// \param points Bezier curve control points.
void DrawCoordinates(const std::vector<Point>& points);

/// \brief Draw Bezier control lines.
/// \param lines Bezier control lines.
void DrawBezierControlLines(const std::vector<std::vector<Point>>& lines);


/// \brief Draw slide control.
void DrawControls(double step, Point* draggingPoint);

/// \brief Draw help text.
void DrawHelp();


/// \brief Draw copyright.
void DrawCopyright();

#endif
