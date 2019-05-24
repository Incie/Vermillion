#pragma once

#include<string>
#include<vector>

class TextService;
class Actor;
class Level;

class InitiativeTracker
{
public:
	InitiativeTracker();
	~InitiativeTracker();

	void CalculateRoundOrder(Level& level);
	
	bool EnemyTurn();
	bool RoundFinished();
	Actor* NextActor();

	int Count() { return (int)initiativeOrder.size(); }
	int CurrentInitiativeIndex() { return currentTurnIndex; }
	const std::vector<Actor*>& Initiatives() { return initiativeOrder; }

private:
	void ClearInitiatives();

	bool enemyTurn;
	int currentTurnIndex;
	std::vector<Actor*> initiativeOrder;
	std::vector<std::string> initiativeNames;
	std::vector<std::string> initiativeGroups;
};