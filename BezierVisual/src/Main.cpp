// Copyright (C) 2018 - 2023 Tony's Studio. All rights reserved.

#include "HelpInterface.h"
#include "Macros.h"
#include "MainInterface.h"


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                      _In_opt_ HINSTANCE hPrevInstance,
                      _In_ LPWSTR lpCmdLine,
                      _In_ int nCmdShow)
{
    // Though we don't use speaker, we have to initialize it.
    Speaker::GetInstance()->Create();

    // Initialization.
    Device::GetInstance()->SetWidth(WIDTH)->SetHeight(HEIGHT)->Create(false);
    setbkcolor(WHITE);
    setbkmode(TRANSPARENT);
    auto app = Application::GetInstance();
    if (!app->Load("UIConfig.xml"))
    {
        return -1;
    }
    if (!app->Initialize())
    {
        return -2;
    }

    app->Run();

    return 0;
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
    {
        intf = new MainInterface();
    }
    else if (_STR_SAME(name, "Help"))
    {
        intf = new HelpInterface();
    }
    else
    {
        LOG_ERROR(INVALID_ATTRIBUTE, "type", name);
    }

    if (intf)
    {
        if (!intf->Load(node))
        {
            delete intf;
            return nullptr;
        }
    }

    return intf;
}
