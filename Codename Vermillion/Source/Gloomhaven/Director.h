#pragma once

#include<vector>
#include<string>
#include"enemyai/EnemyAI.h"
#include"director/Initiative.h"

class InputService;
class TextService;
class Level;
class Action;
class EnemyRound;
class PlayerRound;

class Director {
public:
	Director(Level& level, std::function<void(int)> onEvent);
	~Director();

	void Update(const InputService& input);
	void Render();
	void RenderUI(const TextService& text);

	void StartRound();
	void EndPlayerTurn();

	void SetPlayerRound();
	void AdvanceEnemy();
private:
	void PlayerTurn(const InputService& input);

	void NextActor();

	InitiativeTracker initiativeTracker;
	EnemyAI enemyAi;
	PlayerRound* playerRound;
	Level& level;
	Action* action;
	EnemyRound* enemyRound;
	std::vector<int> turnOrder;
	std::vector<std::string> turnOrderNames;

	std::function<void(int)> onEvent;
};