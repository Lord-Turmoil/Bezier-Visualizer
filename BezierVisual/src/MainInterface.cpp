#include "MainInterface.h"

#include "BezierImpl.h"

const Rect MainInterface::ACTIVE_RANGE(0, 0, 600, 550);


MainInterface::MainInterface() : _impl(new BezierImpl())
{
    _drawPanel.GetImage()->Resize(600, 550);
    Device::GetInstance()->SetTargetImage(_drawPanel.GetImage());
    setbkcolor(WHITE);
    setbkmode(TRANSPARENT);
    Device::GetInstance()->SetTargetImage();

    _drawPanel.SetCoord(0, 0);
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
    device->SetTargetImage();

    Device::GetInstance()->AddSymbol(&_drawPanel);

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
            ->OnTriggered([this] { _impl->ClearControlPoints(); });
    dynamic_cast<KeyboardDetector*>(m_pWidgetManager->GetWidget("toggle-coord"))
            ->OnTriggered([this] { _impl->ToggleShowCoordinate(); });
    dynamic_cast<Slider*>(m_pWidgetManager->GetWidget("step-slider"))
            ->OnChange([this](auto&& ph1) { _OnSlide(std::forward<decltype(ph1)>(ph1)); });
}


void MainInterface::Update(Event* evnt)
{
    AbstractInterface::Update(evnt);
    _ProcessEvent(evnt);
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
            const auto candidate = _impl->FindCandidatePoint(evnt->MouseX(), evnt->MouseY());
            _impl->SetDraggingPoint(candidate);
            if (!candidate)
            {
                _impl->AddControlPoint(evnt->MouseX(), evnt->MouseY());
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
    static char buffer[32];

    _impl->SetStep(value);

    sprintf_s(buffer, "t = %.2f", value);
    dynamic_cast<TextDrawer*>(
        dynamic_cast<StaticWidget*>(
            m_pWidgetManager->GetWidget("step-text"))
        ->GetDrawer())->SetText(buffer);
}

