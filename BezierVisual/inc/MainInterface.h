#pragma once

#include "Macros.h"
#include <dungine.h>

#ifndef _BEZIERVISUAL_MAIN_INTERFACE_H_
#define _BEZIERVISUAL_MAIN_INTERFACE_H_

class BezierImpl;


class MainInterface final : public AbstractInterface
{
public:
    MainInterface();
    ~MainInterface() override;

    void Draw() override;
    void Draw(IMAGE* pDestImage) override;

    void BindEvents() override;

    void Update(Event* evnt) override;

private:
    bool _IsInValidRange(int x, int y);
    void _ProcessEvent(Event* evnt) const;

    void _OnSlide(double value) const;
    void _OnInterpolate() const;

private:
    BezierImpl* _impl;
    BezierImpl* _result;

    Symbol _drawPanel;
    Symbol _resultPanel;

private:
    static const Rect ACTIVE_RANGE;
};



#endif
