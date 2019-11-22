#pragma once

#include"activity.h"
#include "opengl2.h"
#include "window.h"
#include "input.h"
#include "text.h"
#include "textures.h"
#include "timer.h"

class Vermillion
{
public:
	Vermillion(Activity* activity, HINSTANCE);
	~Vermillion();

	void InitializeEngine();
	void DeinitializeEngine();

	void Run();

private:
	Activity* runningActivity;

	Window window;
	GL2Renderer renderer;
	InputManager inputManager;

	Text text;
	TextureManagerGL textureManager;
	ServiceLocator serviceLocator;
	Timer renderTimer;
	Timer fpsTimer;

	HINSTANCE hInstance;
};
