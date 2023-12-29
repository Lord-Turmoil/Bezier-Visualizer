// Copyright (C) 2018 - 2023 Tony's Studio. All rights reserved.

#include "HelpInterface.h"


void HelpInterface::BindEvents()
{
    dynamic_cast<KeyboardDetector*>(m_pWidgetManager->GetWidget("exit-1"))
            ->OnTriggered([this] { m_pSupIntf->ClearSubInterface(); });
    dynamic_cast<KeyboardDetector*>(m_pWidgetManager->GetWidget("exit-2"))
            ->OnTriggered([this] { m_pSupIntf->ClearSubInterface(); });
    dynamic_cast<Button*>(m_pWidgetManager->GetWidget("github"))
            ->OnClick([this] { _OpenGitHub(); });

}


void HelpInterface::_OpenGitHub()
{
    system("start https://github.com/Lord-Turmoil/Bezier-Visualizer");
}
