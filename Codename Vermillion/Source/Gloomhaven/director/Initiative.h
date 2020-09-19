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
	const std::vector<std::pair<Actor*, bool>>& Initiatives() { return initiativeOrder; }

	void ResetOnNextActor() { resetOnNextActor = true; }

	void ClearInitiatives();
private:

	bool resetOnNextActor;

	bool enemyTurn;
	int currentTurnIndex;
	std::vector< std::pair<Actor*, bool> > initiativeOrder;
	//std::vector<std::string> initiativeNames;
	//std::vector<std::string> initiativeGroups;
};