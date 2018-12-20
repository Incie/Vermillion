#include<Windows.h>

#include"Framework\window.h"
#include"Framework\opengl2.h"
#include"Framework\log.h"
#include"Framework\timer.h"
#include"Framework\input.h"
#include"Framework\text.h"
#include"Framework\servicelocator.h"
#include"Framework\textures.h"

#include"Test\GamepadTest.h"
#include"Test\Testing.h"
#include"Test\Gloom.h"

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

	Text text;
	text.Init();

	TextureManager textureManager;

	Timer renderTimer;
	renderTimer.LimitByMilliseconds(17);

	Timer fpsTimer;
	fpsTimer.LimitByMilliseconds(1000);

	Gloom testing;
	//Testing testing;
	//GamepadTest testing;
	
	testing.Initialize();

	unsigned int fps = 0;
	unsigned int fpsCounter = 0;

	ServiceLocator serviceLocator;
	ServiceAssigner serviceAssigner(serviceLocator);
	serviceAssigner.SetTextService(text);
	serviceAssigner.SetTextureService(textureManager);
	serviceAssigner.SetInputService(inputManager);

	testing.SetServiceLocator(serviceLocator);

	bool quitProgram = false;
	while (!quitProgram) {

		quitProgram = window.ProcessMessages();

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
			text.Print(0, 0, fmt::format("FPS: {0}", fps), 24, Colorf(1.0f) );
			renderer.EndFrame();

			fpsCounter++;
		}

		Sleep(0);
	}
	
	Log::Info("Application", "Shutting down");
	
	testing.Deinitialize();

	text.Deinit();
	textureManager.UnloadAll();

	renderer.DestroyRenderContext();
	window.Destroy();

	return 0;
}