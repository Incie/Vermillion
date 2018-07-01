#include<Windows.h>
#include"Framework/window.h"
#include"Framework/opengl2.h"
#include"Framework/log.h"

#include"Test\Testing.h"

int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrev, LPSTR cmdLine, int nShow) 
{
	Log::Info("", "");
	Log::Info("Application", "Starting...");

	Window window;
	GL2Renderer renderer;
	FrameworkPointers::window = &window;
	FrameworkPointers::renderer = &renderer;


	window.Create(hInstance);

	renderer.SetHandles(window.hwnd());
	renderer.CreateRenderContext();

	window.Show();

	Testing testing;

	MSG msg;
	bool quitProgram = false;
	while (!quitProgram) {
		while(PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) 
		{
			if (msg.message == WM_QUIT) {
				Log::Info("Application", "WM_QUIT Posted");
				quitProgram = true;
				break;
			}

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		renderer.StartFrame();
		testing.Update();
		testing.Render();
		renderer.EndFrame();
		Sleep(0);
	}


	Log::Info("Application", "Shutting down");
	renderer.DestroyRenderContext();
	window.Destroy();


	return 0;
}