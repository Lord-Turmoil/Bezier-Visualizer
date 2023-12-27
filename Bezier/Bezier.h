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

/// \brief Calculate curvature at the starting point and the ending point of Bezier curve.
/// \param points Bezier curve control points.
/// \param c0 Out curvature at the starting point.
/// \param c1 Out Curvature at the ending point.
/// \return Whether curvature is valid.
bool CalculateCurvature(const std::vector<Point>& points, double* c0, double* c1);

#endif // _BEZIER_UTILS_H_
