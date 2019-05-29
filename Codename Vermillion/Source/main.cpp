#include"pch.h"
#include<Windows.h>

#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>

#include"Test/GamepadTest.h"
#include"Test/Testing.h"
#include"Gloomhaven/Gloom.h"
#include"Gloomhaven_Leveleditor/Gloom_Editor.h"
#include"Test/CardRendering.h"
#include"Framework/vermillion.h"

//CardRendering testing;
//Gloom activity;
//Testing testing;
//GamepadTest testing;

int __stdcall WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrev, _In_ LPSTR cmdLine, _In_ int nShow)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_DEBUG);

	Activity* activity = nullptr;

	//activity = new CardRendering();
	activity = new GloomEditor();
	//activity = new Gloom();

	auto vermillion = Vermillion(activity, hInstance);
	vermillion.Run();

	return 0;
}
