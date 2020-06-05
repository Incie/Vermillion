#include<Windows.h>
#include<crtdbg.h>
#include<memory>

#include"log.h"

#include"fmt/format.h"
#include"vermillion.h"

#include"sandbox.h"
#include"menutest.h"
#include"activitychess.h"

int __stdcall WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrev, _In_ LPSTR cmdLine, _In_ int nShow)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_DEBUG);

	try {
		auto vermillion = Vermillion(hInstance);

		vermillion.RegisterActivityFactory([](const std::string& activityId) -> std::unique_ptr<Activity> {
			if(activityId == "MenuTest")
				return std::make_unique<MenuTest>();

			if (activityId == "Sandbox")
				return std::make_unique<Sandbox>();

			if(activityId == "ActivityChess")
				return std::make_unique<ActivityChess>();

			throw std::string(fmt::format("Could not find activity by id '{}'", activityId));
			}
		);

		vermillion.StartActivity("Sandbox");
		vermillion.Run();
		vermillion.DeinitializeEngine();
	}
	catch (const std::string exception) {
		Log::Error("Exception Caught", exception);
		MessageBox(nullptr, std::wstring(exception.begin(), exception.end()).c_str(), L"Fatal Exception", MB_OK | MB_ICONERROR | MB_DEFAULT_DESKTOP_ONLY);
	}

	return 0;
}
