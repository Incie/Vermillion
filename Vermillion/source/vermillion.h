#pragma once

#include"activity.h"
#include"opengl2.h"
#include"window.h"
#include"input.h"
#include"text.h"
#include"textures.h"
#include"timer.h"
#include<functional>

class Vermillion
{
public:
	Vermillion(HINSTANCE hInstance);
	Vermillion(Activity* activity, HINSTANCE hInstance);
	~Vermillion();

	void ActivityFactory(std::function<Activity*(const std::string&)> activityFactory);
	void StartActivity(const std::string& activityId);

	void InitializeEngine();
	void DeinitializeEngine();

	void Run();

private:
	std::vector<Activity*> activities;
	std::function<Activity*(const std::string&)> activityFactory;

	Window window;
	GL2Renderer renderer;
	InputManager inputManager;

	TextSDF text;
	TextureManagerGL textureManager;
	ServiceLocator serviceLocator;
	Timer renderTimer;
	Timer fpsTimer;

	HINSTANCE hInstance;
};
