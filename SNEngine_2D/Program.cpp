#include "stdafx.h"
#include "Core\Window.h"
#include "Core\SceneManager.h"

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

	auto scene_manager = std::make_unique<SceneManager>();
	scene_manager->Initialize();

	while (Window::Update())
	{
		scene_manager->Update();
		scene_manager->Render();
	}

	Window::Destroy();
	return 0;
}