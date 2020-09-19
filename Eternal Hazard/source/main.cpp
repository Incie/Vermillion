#include<Windows.h>
#include<crtdbg.h>
#include<memory>

#include"log.h"

#include"fmt/format.h"
#include"vermillion.h"

#include"MainMenu/ActivityMainMenu.h"
#include"Game/ActivitySideScroller.h"


int __stdcall WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrev, _In_ LPSTR cmdLine, _In_ int nShow)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_DEBUG);

	try {
		auto vermillion = Vermillion(hInstance);

		auto activityFactory = [](const std::string& activityId) -> std::unique_ptr<Activity> {
			if(activityId == "MainMenu")
				return std::make_unique<ActivityMainMenu>();

			if(activityId == "ActivitySideScroller")
				return std::make_unique<ActivitySideScroller>();

			throw std::string(fmt::format("Could not find activity by id '{}'", activityId));
		};

		vermillion.RegisterActivityFactory(activityFactory);

		vermillion.StartActivity("MainMenu");
		vermillion.Run();
		vermillion.DeinitializeEngine();
	}
	catch(const std::string exception) {
		Log::Error("Exception Caught", exception);
		MessageBox(nullptr, std::wstring(exception.begin(), exception.end()).c_str(), L"Fatal Exception", MB_OK | MB_ICONERROR | MB_DEFAULT_DESKTOP_ONLY);
	}

	return 0;
}
