#pragma once

#include"activity.h"
#include"camera2d.h"

#include"enemyai/EnemyAI.h"
#include"level/Level.h"
#include"Director.h"
#include"cards/CardGenerator.h"

class UIView;
class Action;

class Gloom : public Activity
{
public:
	Gloom();
	~Gloom();

	void Initialize() override;
	void Deinitialize() override;

	void Resize(const glm::ivec2& newWindowSize) override;

	void Update(float deltaTime) override;
	void Render() override;

private:
	void InitializeUI();
	void OnDirectorEvent(DirectorEvent event);

	Director director;
	Camera2D camera;
	Level level;

	CardGenerator cardGenerator;
};