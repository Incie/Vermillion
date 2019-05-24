#pragma once

#include"../Framework/activity.h"
#include"../Framework/camera2d.h"

#include"enemyai/EnemyAI.h"
#include"level/Level.h"
#include"Director.h"
#include"cards/CardGenerator.h"

class UILayer;
class Action;

class Gloom : public Activity
{
public:
	Gloom();
	~Gloom();

	void Initialize();
	void Deinitialize();

	void Resize();

	void Update(double deltaTime);
	void Render();

private:
	void InitializeUI();
	void OnDirectorEvent(DirectorEvent event);
	std::vector<UILayer*> layers;
	std::vector<PlayerCard> cards;

	Director director;
	Camera2D camera;
	Level level;

	CardGenerator cardGenerator;
};