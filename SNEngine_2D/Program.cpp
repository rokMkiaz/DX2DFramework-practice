#include "stdafx.h"
#include "Core\Window.h"
#include "Core\Engine.h"

int APIENTRY WinMain
(
	HINSTANCE hInstance,
	HINSTANCE prevInstance,
	LPSTR lpszCmdParam,
	int nCmdShow
)
{
	Window::Create(hInstance, 1000, 1000);
	Window::Show();

	Settings::Get().SetWindowHandle(Window::global_handle);
	Settings::Get().SetWidth(static_cast<float>(Window::GetWidth()));
	Settings::Get().SetHeight(static_cast<float>(Window::GetHeight()));

	auto engine= std::make_unique<Engine>();

	while (Window::Update())
	{
		engine->Update();
		engine->Render();
	}

	Window::Destroy();
	return 0;
}