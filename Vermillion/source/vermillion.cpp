#include"pch.h"
#include"vermillion.h"
#include "log.h"
#include "window.h"
#include "text.h"
#include "windowstate.h"

#include<chrono>

#include"vulkan/vulkan.h"
VulkanRenderer vrenderer;

#define LogTag "Vermillion"

Vermillion::Vermillion(HINSTANCE hInstance)
	: hInstance(hInstance)
{
	TRACE(LogTag);
	InitializeEngine();
}

Vermillion::Vermillion(std::unique_ptr<Activity> startActivity, HINSTANCE hInstance)
	: hInstance(hInstance)
{
	TRACE(LogTag)
	InitializeEngine();
	StartActivityNow(startActivity, serviceLocator);
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
			
			//renderer.StartFrame();
				vrenderer.DrawFrame();
				runningActivity->Render();
				//textSdf.Print(0, 0, fmt::format("FPS: {0} ({1}ms)", fps, lastFrameTime), 12, Colors::White );
			//renderer.EndFrame();

			fpsCounter++;
			lastFrameTime = renderTimer.TimeSinceTickAsMilliseconds();

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

	vrenderer.SetHandles(window.hwnd());
	vrenderer.Initialize();
	//renderer.SetHandles(window.hwnd());
	//renderer.CreateRenderContext();

	window.Show();

	//textSdf.Init(textureManager);
	//textSdf.SetServiceId(0);
	//text.Init();
	//text.SetServiceId(1);

	renderTimer.LimitByFPS(59);
	fpsTimer.LimitByMilliseconds(1000);

	ServiceAssigner serviceAssigner(serviceLocator);
	//serviceAssigner.SetTextService(textSdf);
	//serviceAssigner.SetTextService(text);
	serviceAssigner.SetTextureService(textureManager);
	serviceAssigner.SetInputService(inputManager);

	Log::Info("Vermillion", "Engine initialized");
}

void Vermillion::DeinitializeEngine()
{
	TRACE(LogTag);

	DeinitializeActivities();

	// text.Deinit();
	textureManager.UnloadAll();

	vrenderer.Deinitialize();
	renderer.DestroyRenderContext();
	window.Destroy();

	Log::Info("Vermillion", "Shut down");
}