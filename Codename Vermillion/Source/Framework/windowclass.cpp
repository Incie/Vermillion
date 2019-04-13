#include"window.h"
#include"log.h"
#include<string>
#include"fmt\format.h"

Window::WindowClass::WindowClass()
{
	className = "projectvermillion_wndclass";
}

void Window::WindowClass::Create(HINSTANCE hInstance, WNDPROC wndProc)
{
	WNDCLASSEX wndclass = { 0 };
	wndclass.cbSize = sizeof(wndclass);
	wndclass.cbWndExtra = 0;
	wndclass.cbClsExtra = 0;
	wndclass.lpszMenuName = nullptr;
	wndclass.lpszClassName = className.c_str();
	wndclass.hbrBackground = CreateSolidBrush(RGB(0, 0, 0));
	wndclass.hIconSm = nullptr;
	wndclass.hIcon = nullptr;
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.style = CS_VREDRAW | CS_HREDRAW | CS_OWNDC;
	wndclass.hInstance = hInstance;
	wndclass.lpfnWndProc = wndProc;

	Log::Info("WindowClass", "Creating WNDCLASSEX");
	if (RegisterClassEx(&wndclass) == 0) {
		Log::Error("WindowClass", fmt::format("Error Creating WNDCLASSEX. GetLastErrorCode: {0}", GetLastError()));
	}

	this->hInstance = hInstance;
}

void Window::WindowClass::Destroy()
{
	Log::Info("WindowClass", "UnregisterClass");
	UnregisterClass(className.c_str(), hInstance);

	hInstance = nullptr;
	className.clear();
}

const std::string& Window::WindowClass::classname()
{
	return className;
}