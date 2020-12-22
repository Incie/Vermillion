#include<Windows.h>
#include<crtdbg.h>
#include<memory>

#include"log.h"

#include"fmt/format.h"
#include"vermillion.h"

#include"activitymapeditor.h"

int __stdcall WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrev, _In_ LPSTR cmdLine, _In_ int nShow)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_DEBUG);

	try {
		auto vermillion = Vermillion(hInstance);

		vermillion.RegisterActivityFactory([](const std::string& activityId) -> std::unique_ptr<Activity> {
			//if(activityId == "")
			//	return std::make_unique<>();

			if(activityId == "MapEditor")
				return std::make_unique<ActivityMapEditor>();

			throw std::string(fmt::format("Could not find activity by id '{}'", activityId));
			}
		);

		vermillion.StartActivity("MapEditor");
		vermillion.Run();
		vermillion.DeinitializeEngine();
	}
	catch(const std::string exception) {
		Log::Error("Exception Caught", exception);
		MessageBox(nullptr, std::wstring(exception.begin(), exception.end()).c_str(), L"Fatal Exception", MB_OK | MB_ICONERROR | MB_DEFAULT_DESKTOP_ONLY);
	}

	return 0;
}


/*
#pragma once


#include"activity.h"


class Sandbox : public Activity
{
public:
	Sandbox();
	virtual ~Sandbox();

	void Initialize() override;
	void Deinitialize() override;
	void Update(float delta) override;
	void Render() override;

private:
};


Sandbox::Sandbox()
{
}

Sandbox::~Sandbox()
{
}

void Sandbox::Initialize()
{
}

void Sandbox::Deinitialize()
{
}

void Sandbox::Update(float delta)
{
}

void Sandbox::Render()
{
}

*/