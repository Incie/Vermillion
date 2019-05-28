#include"pch.h"
#include"vermillion.h"
#include "log.h"
#include "window.h"
#include "text.h"
#include "windowstate.h"

Vermillion::Vermillion(Activity* activity, HINSTANCE hInstance)
	: runningActivity(activity), hInstance(hInstance)
{
	InitializeEngine();
	
	runningActivity->SetServiceLocator(serviceLocator);
	runningActivity->Initialize();
}

Vermillion::~Vermillion()
{
	if( runningActivity )
	{
		runningActivity->Deinitialize();
		delete runningActivity;
	}

	DeinitializeEngine();
}


void Vermillion::Run()
{
	unsigned int fps = 0;
	unsigned int fpsCounter = 0;

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
				runningActivity->Resize();
				WindowState::ResetChanged();
			}

			runningActivity->Update(renderTimer.GetDelta());
			runningActivity->Render();
			text.Print(0, 0, fmt::format("FPS: {0} ({1})", fps, lastFrameTime), 12, Colorf(1.0f) );
			renderer.EndFrame();

			fpsCounter++;

			lastFrameTime = renderTimer.TimeSinceTick();
		}

		//Sleep(0);
	}
}


void Vermillion::InitializeEngine()
{
	Log::Info("", "");
	Log::Info("Application", "Starting...");

	FrameworkPointers::window = &window;
	FrameworkPointers::renderer = &renderer;
	FrameworkPointers::inputManager = &inputManager;

	window.Create(hInstance);

	renderer.SetHandles(window.hwnd());
	renderer.CreateRenderContext();

	window.Show();

	text.Init();

	renderTimer.LimitByMilliseconds(0);
	fpsTimer.LimitByMilliseconds(1000);

	ServiceAssigner serviceAssigner(serviceLocator);
	serviceAssigner.SetTextService(text);
	serviceAssigner.SetTextureService(textureManager);
	serviceAssigner.SetInputService(inputManager);
}

void Vermillion::DeinitializeEngine()
{
	Log::Info("Application", "Shutting down");
	
	text.Deinit();
	textureManager.UnloadAll();

	renderer.DestroyRenderContext();
	window.Destroy();
}