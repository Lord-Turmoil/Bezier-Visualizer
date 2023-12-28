#include "HelpInterface.h"


void HelpInterface::BindEvents()
{
    dynamic_cast<KeyboardDetector*>(m_pWidgetManager->GetWidget("exit"))
        ->OnTriggered([this] { m_pSupIntf->ClearSubInterface(); });
}
