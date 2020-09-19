#include<Windows.h>
#include<crtdbg.h>

#include"vermillion.h"
#include"gloom/gloomhaven.h"
#include"gloom/Gloom.h"

int __stdcall WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrev, _In_ LPSTR cmdLine, _In_ int nShow)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_DEBUG);

	auto vermillion = Vermillion(std::make_unique<Gloom>(), hInstance);
	vermillion.Run();

	return 0;
}
