#include"pch.h"
#include"vermillion.h"
#include "log.h"
#include "window.h"
#include "text.h"
#include "windowstate.h"

#define LogTag "Vermillion"

Vermillion::Vermillion(HINSTANCE hInstance)
	: hInstance(hInstance)
{
	TRACE(LogTag);
	InitializeEngine();
}

Vermillion::Vermillion(std::unique_ptr<Activity> activity, HINSTANCE hInstance)
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

	fpsTimer.Reset();
	renderTimer.Reset();

	while (!quitProgram) {
		quitProgram = window.ProcessMessages();

		if (fpsTimer.Tick()) {
			fps = fpsCounter;
			fpsCounter = 0;
		}

		if( renderTimer.Tick() )
		{
			float deltaTime = static_cast<float>(renderTimer.GetDelta());

			inputManager.Update();

			//Should be "HasQueuedEngineJob" at a later point
			if(HasQueuedActivity())
				SetupQueuedActivity(serviceLocator);

			auto& runningActivity = ActiveActivity();

			if (WindowState::Changed()) {
				runningActivity->Resize(WindowState::Size());
				WindowState::ResetChanged();
			}

			runningActivity->Update(deltaTime);
			
			renderer.StartFrame();
				runningActivity->Render();
				textSdf.Print(0, 0, fmt::format("FPS: {0} ({1}us [{2}])", fps, lastFrameTime, static_cast<uint32_t>(1.0 / (lastFrameTime / 1e6))), 12, Colors::White );
			renderer.EndFrame();

			fpsCounter++;

			if( fpsCounter % 5 == 0 )
				lastFrameTime = renderTimer.TimeSinceTickAsMicroseconds();

			if(runningActivity->Finished()) {
				Log::Info("Run()", "Activity finished");
				quitProgram = true;
			}
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

	textSdf.Init(textureManager);
	textSdf.SetServiceId(0);
	text.Init();
	text.SetServiceId(1);

	renderTimer.LimitByFPS(59);
	fpsTimer.LimitByMilliseconds(1000);

	ServiceAssigner serviceAssigner(serviceLocator);
	serviceAssigner.SetTextService(textSdf);
	serviceAssigner.SetTextService(text);
	serviceAssigner.SetTextureService(textureManager);
	serviceAssigner.SetInputService(inputManager);

	Log::Info(LogTag, "Engine initialized");
}

void Vermillion::DeinitializeEngine()
{
	TRACE(LogTag);

	DeinitializeActivities();

	textSdf.Deinit();
	text.Deinit();
	textureManager.UnloadAll();

	renderer.DestroyRenderContext();
	window.Destroy();

	Log::Info(LogTag, "Shut down");
}