#pragma once

class Level;
class Actor;
class EnemyRound;
class TextService;
class Director;

#include<vector>
#include"glm/glm.hpp"

class EnemyAI
{
public:
	EnemyAI();
	~EnemyAI();

	void SetRoundActions(EnemyRound* enemyRound);
	void SetActor(Actor* actor);
	Actor* GetActor() { return actor; }
	
	bool Finished();
	void Step(Director& director, Level& level);

	void Render();

private:
	void CalculateStep(Level& level);
	void PerformStep(Director& director, Level& level);

	EnemyRound* enemyRound;
	Actor* actor;
};