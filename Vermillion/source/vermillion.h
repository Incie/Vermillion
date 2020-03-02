#pragma once

#include"activity.h"
#include"opengl2.h"
#include"window.h"
#include"input.h"
#include"text.h"
#include"textures.h"
#include"timer.h"
#include<functional>

//todo: move class definition to another file
class ActivityInterface {
public:
	ActivityInterface();

	void ActivityFactory(std::function<Activity * (const std::string&)> activityFactory);
	void StartActivity(const std::string& activityId);

	Activity* ActiveActivity();

protected:
	void StartActivityNow(Activity* activity, ServiceLocator& serviceLocator);
	bool HasQueuedActivity();
	void SetupQueuedActivity(ServiceLocator& serviceLocator);
	void DeinitializeActivities();

private:
	std::string toBeStarted;
	std::vector<Activity*> activities;
	std::function<Activity* (const std::string&)> activityFactory;
};


class Vermillion : public ActivityInterface
{
public:
	Vermillion(HINSTANCE hInstance);
	Vermillion(Activity* activity, HINSTANCE hInstance);
	~Vermillion();

	void InitializeEngine();
	void DeinitializeEngine();

	void Run();

private:
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
