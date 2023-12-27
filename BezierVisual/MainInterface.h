#pragma once

#include "Macros.h"
#include <dungine.h>

#ifndef _BEZIERVISUAL_MAIN_INTERFACE_H_
#define _BEZIERVISUAL_MAIN_INTERFACE_H_

class MainInterface final : public AbstractInterface
{
public:
    MainInterface() = default;
    ~MainInterface() override = default;

    void BindEvents() override;

    void Update(Event* evnt) override;

private:
    void _OnSlide(double value);

private:
    std::vector<Point> _controlPoints;
    std::vector<std::vector<Point>> _controlLines;
    Point* _draggingPoint;

    double _step;

    bool _showCoordinate;
    bool _showCurvature;
};



#endif
