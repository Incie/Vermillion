#pragma once

class Level;
class Actor;
class EnemyRound;
class TextService;

#include<vector>
#include"glm/glm.hpp"

class EnemyAI
{
public:
	EnemyAI(Level& level);
	~EnemyAI();

	void SetRoundActions(EnemyRound* enemyRound);
	void SetActor(Actor* actor);
	
	void CalculateStep();
	void PerformStep();
	void Render();

private:
	EnemyRound* enemyRound;
	Actor* actor;
	Level& level;
};