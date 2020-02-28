#include"pch.h"
#include"vermillion.h"
#include "log.h"
#include "window.h"
#include "text.h"
#include "windowstate.h"

#include<chrono>

#define LogTag "Vermillion"

Vermillion::Vermillion(HINSTANCE hInstance)
	: hInstance(hInstance)
{
	TRACE(LogTag);
	InitializeEngine();
}

Vermillion::Vermillion(Activity* activity, HINSTANCE hInstance)
	: hInstance(hInstance)
{
	TRACE(LogTag)
	InitializeEngine();
	StartActivityNow(activity, serviceLocator);
}

Vermillion::~Vermillion()
{
	TRACE(LogTag)
	DeinitializeEngine();
}

void Vermillion::Run()
{
	TRACE(LogTag)

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

			//Should be "HasQueuedEngineJob" at a later point
			if(HasQueuedActivity())
				SetupQueuedActivity(serviceLocator);

			auto runningActivity = ActiveActivity();

			if (WindowState::Changed()) {
				runningActivity->Resize(WindowState::Size());
				WindowState::ResetChanged();
			}

			float deltaTime = static_cast<float>(renderTimer.GetDelta());
			runningActivity->Update(deltaTime);
			
			renderer.StartFrame();
				runningActivity->Render();
				text.Print(0, 0, fmt::format("FPS: {0} ({1}ms)", fps, lastFrameTime), 12, Colors::White );
			renderer.EndFrame();

			fpsCounter++;
			lastFrameTime = renderTimer.TimeSinceTickAsMilliseconds();
		}
		else
			Sleep(0);
	}
}


void Vermillion::InitializeEngine()
{
	Log::Info("", "");
	TRACE(LogTag)

	FrameworkPointers::window = &window;
	FrameworkPointers::renderer = &renderer;
	FrameworkPointers::inputManager = &inputManager;

	window.Create(hInstance);

	renderer.SetHandles(window.hwnd());
	renderer.CreateRenderContext();

	window.Show();

	text.Init(textureManager);

	renderTimer.LimitByFPS(55);
	fpsTimer.LimitByMilliseconds(1000);

	ServiceAssigner serviceAssigner(serviceLocator);
	serviceAssigner.SetTextService(text);
	serviceAssigner.SetTextureService(textureManager);
	serviceAssigner.SetInputService(inputManager);

	Log::Info("Vermillion", "Engine initialized");
}

void Vermillion::DeinitializeEngine()
{
	TRACE(LogTag);

	DeinitializeActivities();

	text.Deinit();
	textureManager.UnloadAll();

	renderer.DestroyRenderContext();
	window.Destroy();

	Log::Info("Vermillion", "Shut down");
}