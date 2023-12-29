#include "MainInterface.h"

#include "BezierImpl.h"

const Rect MainInterface::ACTIVE_RANGE(0, 0, 600, 550);
static char buffer[32];

MainInterface::MainInterface() : _impl(new BezierImpl()), _result(new BezierImpl())
{
    _drawPanel.GetImage()->Resize(600, 550);
    Device::GetInstance()->SetTargetImage(_drawPanel.GetImage());
    setbkcolor(WHITE);
    setbkmode(TRANSPARENT);

    _resultPanel.GetImage()->Resize(600, 550);
    Device::GetInstance()->SetTargetImage(_resultPanel.GetImage());
    setbkcolor(WHITE);
    setbkmode(TRANSPARENT);

    Device::GetInstance()->SetTargetImage();

    _drawPanel.SetCoord(0, 0);
    _resultPanel.SetCoord(600, 0);
}


MainInterface::~MainInterface()
{
    delete _impl;
}


void MainInterface::Draw()
{
    const auto device = Device::GetInstance();
    device->SetTargetImage(_drawPanel.GetImage());
    _impl->Render();
    device->SetTargetImage(_resultPanel.GetImage());
    _result->Render();
    device->SetTargetImage();

    device->AddSymbol(&_drawPanel);
    device->AddSymbol(&_resultPanel);

    AbstractInterface::Draw();
}


void MainInterface::Draw(IMAGE* pDestImage)
{
    const auto device = Device::GetInstance();
    device->SetTargetImage(_drawPanel.GetImage());
    _impl->Render();
    device->SetTargetImage();

    Device::GetInstance()->AddSymbol(&_drawPanel);

    AbstractInterface::Draw(pDestImage);
}


void MainInterface::BindEvents()
{
    dynamic_cast<KeyboardDetector*>(m_pWidgetManager->GetWidget("exit"))
        ->OnTriggered(GetTerminator(this));
    dynamic_cast<KeyboardDetector*>(m_pWidgetManager->GetWidget("help"))
        ->OnTriggered([this] { SetSubInterface(Application::GetInstance()->GetInterface("Help")); });

    dynamic_cast<KeyboardDetector*>(m_pWidgetManager->GetWidget("clear"))
        ->OnTriggered([this] { _impl->ClearControlPoints(); _result->ClearControlPoints(); });
    dynamic_cast<KeyboardDetector*>(m_pWidgetManager->GetWidget("toggle-coord"))
        ->OnTriggered([this] { _impl->ToggleShowCoordinate(); _result->ToggleShowCoordinate(); });
    dynamic_cast<Slider*>(m_pWidgetManager->GetWidget("step-slider"))
        ->OnChange([this](auto&& ph1) { _OnSlide(std::forward<decltype(ph1)>(ph1)); });

    dynamic_cast<Button*>(m_pWidgetManager->GetWidget("interpolate"))
        ->OnClick([this] { _OnInterpolate(); });
}


void MainInterface::Update(Event* evnt)
{
    AbstractInterface::Update(evnt);
    if (!m_pSubIntf)
    {
        _ProcessEvent(evnt);
    }
}


bool MainInterface::_IsInValidRange(int x, int y)
{
    return x >= 0 && x < _drawPanel.GetImage()->getwidth() &&
        y >= 0 && y < _drawPanel.GetImage()->getheight();
}


void MainInterface::_ProcessEvent(Event* evnt) const
{
    if (evnt->MouseDown())
    {
        if (InRectangleStrict(ACTIVE_RANGE, evnt->Mouse()))
        {
            auto candidate = _impl->FindCandidatePoint(evnt->MouseX(), evnt->MouseY());
            _impl->SetDraggingPoint(candidate);
            if (!candidate)
            {
                candidate = _impl->AddControlPoint(evnt->MouseX(), evnt->MouseY());
                _impl->SetDraggingPoint(candidate);
            }
        }
    }
    else if (evnt->Instant(VK_LBUTTON))
    {
        if (InRectangleStrict(ACTIVE_RANGE, evnt->Mouse()))
        {
            if (evnt->Instant(VK_SHIFT))
            {
                _impl->DragPoint(_impl->FindAttachX(evnt->MouseX()), _impl->FindAttachY(evnt->MouseY()));
            }
            else
            {
                _impl->DragPoint(evnt->MouseX(), evnt->MouseY());
            }
        }
    }
    else
    {
        _impl->SetDraggingPoint(nullptr);
    }

    if (evnt->Instant(VK_RBUTTON))
    {
        auto candidate = _impl->FindCandidatePoint(evnt->MouseX(), evnt->MouseY());
        _impl->RemoveControlPoint(candidate);
    }
}


void MainInterface::_OnSlide(double value) const
{
    _impl->SetStep(value);

    sprintf_s(buffer, "t = %.2f", value);
    dynamic_cast<TextDrawer*>(
        dynamic_cast<StaticWidget*>(
            m_pWidgetManager->GetWidget("step-text"))
        ->GetDrawer())->SetText(buffer);
}


void MainInterface::_OnInterpolate() const
{
    double c0;
    double c1;

    const std::vector<Point>& points(_impl->GetControlPoints());
    if (points.size() < 2)
    {
        return;
    }

    CalculateCurvature(points, &c0, &c1);
    Point v0 = points[1] - points[0];
    Point v1 = points[points.size() - 1] - points[points.size() - 2];

    std::vector<Point> interpolatePoints;
    int ret = InterpolateWithBezierCurve(points[0], points[points.size() - 1], v0, v1, c0, c1, &interpolatePoints);
    if (ret < 0)
    {
        sprintf_s(buffer, "Error: %d", ret);
        const auto staticWidget = dynamic_cast<StaticWidget*>(m_pWidgetManager->GetWidget("error"));
        staticWidget->SetAlpha(255);
        dynamic_cast<TextDrawer*>(staticWidget->GetDrawer())->SetText(buffer);

        dynamic_cast<StaticWidget*>(m_pWidgetManager->GetWidget("success"))->SetAlpha(0);
    }
    else
    {
        dynamic_cast<StaticWidget*>(m_pWidgetManager->GetWidget("error"))->SetAlpha(0);
        dynamic_cast<StaticWidget*>(m_pWidgetManager->GetWidget("success"))->SetAlpha(255);
    }

    _result->ClearControlPoints();
    for (const auto& point : interpolatePoints)
    {
        _result->AddControlPoint(point.x, point.y);
    }
}

