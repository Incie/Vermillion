#include"pch.h"
#include<Windows.h>

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include"Framework\window.h"
#include"Framework\opengl2.h"
#include"Framework\log.h"
#include"Framework\timer.h"
#include"Framework\input.h"
#include"Framework\text.h"
#include"Framework\servicelocator.h"
#include"Framework\textures.h"
#include"Framework\windowstate.h"

#include"Test\GamepadTest.h"
#include"Test\Testing.h"
#include"Gloomhaven/Gloom.h"
#include"Test/CardRendering.h"


int __stdcall WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrev, _In_ LPSTR cmdLine, _In_ int nShow)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_DEBUG);

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
	renderTimer.LimitByMilliseconds(0);

	Timer fpsTimer;
	fpsTimer.LimitByMilliseconds(1000);

	unsigned int fps = 0;
	unsigned int fpsCounter = 0;

	ServiceLocator serviceLocator;
	ServiceAssigner serviceAssigner(serviceLocator);
	serviceAssigner.SetTextService(text);
	serviceAssigner.SetTextureService(textureManager);
	serviceAssigner.SetInputService(inputManager);


	//CardRendering testing;
	Gloom testing;
	//Testing testing;
	//GamepadTest testing;

	testing.SetServiceLocator(serviceLocator);
	testing.Initialize();

	double lastFrameTime = 0.0;
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

			if (WindowState::Changed()) {
				testing.Resize();
				WindowState::ResetChanged();
			}

			testing.Update(renderTimer.GetDelta());
			testing.Render();
			text.Print(0, 0, fmt::format("FPS: {0} ({1})", fps, lastFrameTime), 12, Colorf(1.0f) );
			renderer.EndFrame();

			fpsCounter++;

			lastFrameTime = renderTimer.TimeSinceTick();
		}

		//Sleep(0);
	}
	
	Log::Info("Application", "Shutting down");
	
	testing.Deinitialize();

	text.Deinit();
	textureManager.UnloadAll();

	renderer.DestroyRenderContext();
	window.Destroy();

	return 0;
}