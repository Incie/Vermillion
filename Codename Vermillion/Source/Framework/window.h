#pragma once

#include<Windows.h>
#include<string>

class InputManager;
class Window;
class GL2Renderer;
class FrameworkPointers {
public:
	static GL2Renderer* renderer;
	static Window* window;
	static InputManager* inputManager;
};

class Window
{
public:
	Window();

	void Create(HINSTANCE hInstance);
	void Destroy();
	void Show();

	HWND hwnd() { return hWnd; }

private:
	class WindowClass
	{
	public:
		WindowClass();
		void Create(HINSTANCE hInstance, WNDPROC wndProc);
		void Destroy();

		const std::string& classname();

	private:
		std::string className;
		HINSTANCE hInstance;
	};

	HWND hWnd;
	HINSTANCE hInstance;
	HDC hDC;
	Window::WindowClass windowClass;
};


