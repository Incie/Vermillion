#include"pch.h"
#include"window.h"
#include"opengl2.h"
#include"log.h"

#include"inputstate.h"
#include"windowstate.h"


GL2Renderer* FrameworkPointers::renderer;
Window* FrameworkPointers::window;
InputManager* FrameworkPointers::inputManager;

LRESULT CALLBACK WindowProc(_In_ HWND hwnd, _In_ UINT   uMsg, _In_ WPARAM wParam, _In_ LPARAM lParam)
{
	switch (uMsg) {
		case WM_ACTIVATE:
			if(wParam == WA_INACTIVE)
				InputState::Active(false);
			else
				InputState::Active(true);

			break;

		case WM_KEYDOWN:
			InputState::SetKeyDown(wParam & 0xFF);
			break;
		case WM_KEYUP:
			InputState::SetKeyUp(wParam & 0xFF);
			break;

		case WM_SIZE:
		case WM_MOVE:
		{
			RECT clientRect;
			GetClientRect(hwnd, &clientRect);
			MapWindowPoints(hwnd, nullptr, reinterpret_cast<POINT*>(&clientRect), 2);

			int width = clientRect.right - clientRect.left;
			int height = clientRect.bottom - clientRect.top;

			WindowState::SetWindowState(clientRect.left, clientRect.top, width, height);
			FrameworkPointers::renderer->SetViewport(width, height);
			return 0;
		}

		case WM_CLOSE:
		case WM_DESTROY:
			Log::Info("WindowProc", fmt::format("Posting Quit Message. {0}", uMsg) );
			PostQuitMessage(0);
			return 0;

		default:
			break;
	}

	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}