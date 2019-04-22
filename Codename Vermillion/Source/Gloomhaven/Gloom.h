#pragma once

#include"../Framework/activity.h"
#include"../Framework/camera2d.h"

#include"enemyai/EnemyAI.h"
#include"level/Level.h"
#include"Director.h"

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
	Director director;
	Camera2D camera;
	Level level;
};