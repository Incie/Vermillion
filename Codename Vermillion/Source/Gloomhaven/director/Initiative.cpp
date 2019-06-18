#include"pch.h"
#include "Initiative.h"
#include"../services.h"
#include"../entity/Entity.h"
#include"../level/Level.h"

InitiativeTracker::InitiativeTracker() 
	: currentTurnIndex(-1), enemyTurn(false), resetOnNextActor(false)
{
}

InitiativeTracker::~InitiativeTracker()
{
	ClearInitiatives();
}

void InitiativeTracker::CalculateRoundOrder(Level& level)
{
	auto monsters = level.Monsters();

	std::sort(monsters.begin(), monsters.end(), [](auto m1, auto m2) {
		auto enemy1 = dynamic_cast<Enemy*>(m1);
		auto enemy2 = dynamic_cast<Enemy*>(m2);

		if (enemy1->Name().compare(enemy2->Name()) == 0) {
			if (enemy1->Elite() && !enemy2->Elite())
				return true;

			if (!enemy1->Elite() && enemy2->Elite())
				return false;

			return (enemy1->EnemyId() < enemy2->EnemyId());
		}

		return enemy1->Initiative() < enemy2->Initiative();
	});

	auto player = level.GetPlayer();

	ClearInitiatives();

	for(auto a : monsters) 
		initiativeOrder.push_back(std::pair<Actor*, bool>(a, a->HasMoved()));

	initiativeOrder.push_back(std::pair<Actor*, bool>(player, player->HasMoved()));
	std::sort(initiativeOrder.begin(), initiativeOrder.end(), [](auto a1, auto a2) {
		return a1.first->Initiative() < a2.first->Initiative();
	});
}

bool InitiativeTracker::EnemyTurn()
{
	return enemyTurn;
}

bool InitiativeTracker::RoundFinished()
{
	bool validTurnIndex = (currentTurnIndex < 0 || currentTurnIndex >= initiativeOrder.size());
	return validTurnIndex;
}

Actor* InitiativeTracker::NextActor()
{
	currentTurnIndex++;

	if(resetOnNextActor) {
		resetOnNextActor = false;
		currentTurnIndex = 0;
	}

	if (currentTurnIndex >= 0 && currentTurnIndex < initiativeOrder.size()) {
		auto nextActor = initiativeOrder[currentTurnIndex].first;

		if (nextActor->Health() <= 0)
			return NextActor();

		if(nextActor->HasMoved())
			return NextActor();


		enemyTurn = false;
		if (dynamic_cast<Enemy*>(nextActor) != nullptr)
			enemyTurn = true;

		return nextActor;
	}

	return nullptr;
}

void InitiativeTracker::ClearInitiatives()
{
	currentTurnIndex = -1;
	initiativeOrder.clear();
	//initiativeGroups.clear();
	//initiativeNames.clear();
}
