#include<Windows.h>

#include"Framework\window.h"
#include"Framework\opengl2.h"
#include"Framework\log.h"
#include"Framework\timer.h"
#include"Framework\input.h"
#include"Framework\text.h"

#include"Test\GamepadTest.h"
#include"Test\Testing.h"

#include"fmt\format.h"

int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrev, LPSTR cmdLine, int nShow) 
{
	Log::Info("", "");
	Log::Info("Application", "Starting...");

	Window window;
	GL2Renderer renderer;
	InputManager inputManager;
	FrameworkPointers::window = &window;
	FrameworkPointers::renderer = &renderer;
	FrameworkPointers::inputManager = &inputManager;

	window.Create(hInstance);

	renderer.SetHandles(window.hwnd());
	renderer.CreateRenderContext();

	window.Show();

	Text::Init();

	Timer renderTimer;
	renderTimer.LimitByFPS(60);

	Timer fpsTimer;
	fpsTimer.LimitByMilliseconds(1000);

	//Testing testing;
	GamepadTest testing;

	unsigned int fps = 0;
	unsigned int fpsCounter = 0;

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

		if (fpsTimer.Tick()) {
			fps = fpsCounter;
			fpsCounter = 0;
		}



		if( renderTimer.Tick() )
		{
			inputManager.Update();

			renderer.StartFrame();
			testing.Update(renderTimer.GetDelta());
			testing.Render();
			Text::Render(0, 0, fmt::format("FPS: {0}", fps), 18);
			renderer.EndFrame();

			fpsCounter++;
		}


		Sleep(0);
	}



	Log::Info("Application", "Shutting down");

	Text::Deinit();

	renderer.DestroyRenderContext();
	window.Destroy();


	return 0;
}