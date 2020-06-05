#pragma once

#include"activity.h"
#include"opengl2.h"
#include"window.h"
#include"input.h"
#include"text.h"
#include"textures.h"
#include"timer.h"
#include<functional>
#include<memory>

//todo: move class definition to another file
class ActivityInterface {
public:
	ActivityInterface();

	void RegisterActivityFactory(std::function<std::unique_ptr<Activity> (const std::string&)> activityFactory);
	void StartActivity(const std::string& activityId);

	std::unique_ptr<Activity>& ActiveActivity();

protected:
	void StartActivityNow(std::unique_ptr<Activity>& activity, ServiceLocator& serviceLocator);
	bool HasQueuedActivity();
	void SetupQueuedActivity(ServiceLocator& serviceLocator);
	void DeinitializeActivities();

private:
	std::string toBeStarted;
	std::vector<std::unique_ptr<Activity>> activities;
	std::function<std::unique_ptr<Activity> (const std::string&)> activityFactory;
};


class Vermillion : public ActivityInterface
{
public:
	Vermillion(HINSTANCE hInstance);
	Vermillion(std::unique_ptr<Activity> activity, HINSTANCE hInstance);
	~Vermillion();

	void InitializeEngine();
	void DeinitializeEngine();

	void Run();

private:
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
