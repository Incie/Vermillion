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

class Director {
public:
	Director(Level& level);
	~Director();

	void Update(const InputService& input);
	void Render();
	void RenderUI(const TextService& text);
private:

	InitiativeTracker initiativeTracker;
	EnemyAI enemyAi;
	Level& level;
	Action* action;
	EnemyRound* enemyRound;
	std::vector<int> turnOrder;
	std::vector<std::string> turnOrderNames;
};