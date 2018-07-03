#include"window.h"
#include"opengl2.h"
#include"log.h"
#include"fmt\format.h"


GL2Renderer* FrameworkPointers::renderer;
Window* FrameworkPointers::window;

LRESULT CALLBACK WindowProc(_In_ HWND hwnd, _In_ UINT   uMsg, _In_ WPARAM wParam, _In_ LPARAM lParam)
{
	switch (uMsg) {
		case WM_SIZE:
		case WM_MOVE:
		{
			RECT clientRect;
			GetClientRect(hwnd, &clientRect);

			int width = clientRect.right - clientRect.left;
			int height = clientRect.bottom - clientRect.top;

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