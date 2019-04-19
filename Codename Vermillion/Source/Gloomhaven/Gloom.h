#pragma once

#include"../Framework/activity.h"
#include"../Framework/camera2d.h"

#include"level/Level.h"

class Action;

class Gloom : public Activity
{
public:
	Gloom();
	~Gloom();

	void Initialize();
	void Deinitialize();

	void Update(double deltaTime);
	void Render();

private:
	Camera2D camera;
	Level level;
	Action* action;
};