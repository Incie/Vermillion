#include<Windows.h>
#include<crtdbg.h>
#include"log.h"

#include"vermillion.h"
#include"sandbox.h"

int __stdcall WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrev, _In_ LPSTR cmdLine, _In_ int nShow)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_DEBUG);

	auto vermillion = Vermillion(new Sandbox(), hInstance);
	vermillion.Run();
	vermillion.DeinitializeEngine();
	return 0;
}
