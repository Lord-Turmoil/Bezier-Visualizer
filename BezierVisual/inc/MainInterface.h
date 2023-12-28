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

private:
    BezierImpl* _impl;
    Symbol _drawPanel;

private:
    static const Rect ACTIVE_RANGE;
};



#endif
