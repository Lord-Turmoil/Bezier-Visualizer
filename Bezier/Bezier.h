#pragma once

#ifndef _BEZIER_H_
#define _BEZIER_H_

#include <vector>
#include "Defines.h"

/// \brief Calculate the combination number of n and k.
/// \param n The number of elements.
/// \param k The number of elements to be chosen.
/// \return The combination number of n and k.
int Combination(int n, int k);

/// \brief Initialize Bezier curve control lines.
/// \param lines Control lines of Bezier curve.
/// \param step Range in [0, 1], 0 means nothing, 1 means complete.
void InitControlLines(std::vector<std::vector<Point>>* lines, const std::vector<Point>* points, double step);

/// \brief Draw a Bezier curve.
/// \param points Control points of Bezier curve.
/// \param step Range in [0, 1], 0 means nothing, 1 means complete.
void DrawBezierCurve(const std::vector<Point>& points, double step);

/// \brief Draw Bezier control lines.
/// \param lines Bezier control lines.
void DrawBezierControlLines(const std::vector<std::vector<Point>>& lines);

#endif // _BEZIER_UTILS_H_