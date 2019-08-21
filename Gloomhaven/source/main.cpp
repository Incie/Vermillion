#include<Windows.h>
#include<crtdbg.h>

#include"vermillion.h"
#include"gloomhaven.h"

int __stdcall WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrev, _In_ LPSTR cmdLine, _In_ int nShow)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_DEBUG);

	auto vermillion = Vermillion(new GloomhavenGame(), hInstance);
	vermillion.Run();

	return 0;
}
