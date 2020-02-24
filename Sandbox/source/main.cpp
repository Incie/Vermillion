#include<Windows.h>
#include<crtdbg.h>
#include"log.h"

#include"fmt/format.h"
#include"vermillion.h"

#include"sandbox.h"
#include"menutest.h"

int __stdcall WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrev, _In_ LPSTR cmdLine, _In_ int nShow)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_DEBUG);

	try {
		auto vermillion = Vermillion(hInstance);

		vermillion.ActivityFactory([](const std::string& activityId) -> Activity* {

			if (activityId == "MenuTest")
				return new MenuTest();

			if (activityId == "Sandbox")
				return new Sandbox();

			throw std::string(fmt::format("Could not find activity by id '{}'", activityId));
			}
		);

		vermillion.StartActivity("Sandbox");
		vermillion.Run();
		vermillion.DeinitializeEngine();
	}
	catch (const std::string exception) {
		Log::Error("Exception Caught", exception);
		MessageBoxA(nullptr, exception.c_str(), "Fatal Exception", MB_OK);
	}

	return 0;
}
