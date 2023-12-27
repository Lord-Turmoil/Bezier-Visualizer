#include "MainInterface.h"


void MainInterface::BindEvents()
{
    dynamic_cast<KeyboardDetector*>(m_pWidgetManager->GetWidget("exit"))
            ->OnTriggered(GetTerminator(this));
    dynamic_cast<Slider*>(m_pWidgetManager->GetWidget("step-slider"))
            ->OnChange([this](auto&& ph1) { _OnSlide(std::forward<decltype(ph1)>(ph1)); });
}


void MainInterface::Update(Event* evnt)
{
    AbstractInterface::Update(evnt);
}


void MainInterface::_OnSlide(double value)
{
    static char buffer[32];

    _step = value;
    sprintf_s(buffer, "t = %.2f", _step);
    dynamic_cast<TextDrawer*>(
        dynamic_cast<StaticWidget*>(
            m_pWidgetManager->GetWidget("step-text"))
        ->GetDrawer())->SetText(buffer);
}


AbstractInterface* LoadInterface(XMLElement* node)
{
    /*
    **	<IntfName name="">
    **		<Widget>
    **		</Widget>
    **	</IntfName>
    */
    const char* name = node->Name();

    AbstractInterface* intf = nullptr;
    if (_STR_SAME(name, "Main"))
        intf = new MainInterface();
    else
        LOG_ERROR(INVALID_ATTRIBUTE, "type", name);

    if (intf)
    {
        // intf->SetWidgetManager(new WidgetManager());
        if (!intf->Load(node))
        {
            delete intf;
            return nullptr;
        }
    }

    return intf;
}
