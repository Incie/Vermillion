#include"pch.h"
#include "window.h"
#include"log.h"

LRESULT CALLBACK WindowProc(_In_ HWND hwnd, _In_ UINT   uMsg, _In_ WPARAM wParam, _In_ LPARAM lParam);

#define LOGTAG "Window"

Window::Window()
{
}

bool Window::ProcessMessages()
{
	static MSG msg;
	while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		if (msg.message == WM_QUIT) {
			Log::Info("Application", "WM_QUIT Posted");
			return true;
		}

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return false;
}

void Window::Create(HINSTANCE hInstance)
{
	windowClass.Create(hInstance, WindowProc);

	Log::Info(LOGTAG, "Creating Window");
	hWnd = CreateWindow(windowClass.classname().c_str(), "Project Vermillion", WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_OVERLAPPEDWINDOW, 250, 250, 512, 512, nullptr, nullptr, hInstance, nullptr);

	if (hWnd == NULL) {
		Log::Error(LOGTAG, "Error Creating Window");
		std::string error = "GetLastError code " + GetLastError();
		Log::Error(LOGTAG, error.c_str());
		return;
	}

	this->hInstance = hInstance;
}

void Window::Destroy()
{
	if (hWnd != nullptr) {
		Log::Info(LOGTAG, "Destroying Window");
		DestroyWindow(hWnd);
		hWnd = nullptr;
	}

	windowClass.Destroy();
}

void Window::Show()
{
	Log::Info(LOGTAG, "Showing Window");
	ShowWindow(hWnd, SW_SHOW);
}
