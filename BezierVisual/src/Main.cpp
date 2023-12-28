#include "Macros.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                      _In_opt_ HINSTANCE hPrevInstance,
                      _In_ LPWSTR    lpCmdLine,
                      _In_ int       nCmdShow)
{
    Speaker::GetInstance()->Create();

    // Initialization.
    Device::GetInstance()->SetWidth(WIDTH)->SetHeight(HEIGHT)->Create(false);
    setbkcolor(WHITE);
    setbkmode(TRANSPARENT);
    auto app = Application::GetInstance();
    assert(app->Load("UIConfig.xml"));
    assert(app->Initialize());

    app->Run();

    return 0;
}