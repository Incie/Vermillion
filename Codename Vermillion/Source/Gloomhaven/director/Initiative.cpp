#include"pch.h"
#include "Initiative.h"
#include"../services.h"
#include"../entity/Entity.h"
#include"../level/Level.h"

InitiativeTracker::InitiativeTracker() 
	: currentTurnIndex(-1), enemyTurn(false)
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

	initiativeOrder.insert(initiativeOrder.end(), monsters.begin(), monsters.end());	
	initiativeOrder.push_back(player);
	std::sort(initiativeOrder.begin(), initiativeOrder.end(), [](auto a1, auto a2) {
		return a1->Initiative() < a2->Initiative();
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

	if (currentTurnIndex >= 0 && currentTurnIndex < initiativeOrder.size()) {
		auto nextActor = initiativeOrder[currentTurnIndex];

		enemyTurn = false;
		if (dynamic_cast<Enemy*>(nextActor) != nullptr)
			enemyTurn = true;

		return nextActor;
	}

	return nullptr;
}

#include"GL/glew.h"
void InitiativeTracker::Render(const TextService& text)
{
	if (RoundFinished())
		return;

	for (int i = 0; i < initiativeOrder.size(); ++i ) {
		auto a = initiativeOrder[i];

		if (a == nullptr)
			throw fmt::format("InitiativeOrder {0} is null", i);


		if (i == currentTurnIndex) {
			glBegin(GL_LINES);
				glVertex2f(0, 9); glVertex2f(-10, 3);
				glVertex2f(0, 9); glVertex2f(-10, 15);
			glEnd();
		}

		auto color = Colorf(1);
		auto enemy = dynamic_cast<Enemy*>(a);
		if (enemy != nullptr) {
			if( enemy->Elite() )
				color = Colorf(1, 1, 0);
			text.Print(0, 0, fmt::format("({0}) {1}({2})", enemy->Initiative(), enemy->Name(), enemy->EnemyId()), 18, color);
			continue;
		}

		auto player = dynamic_cast<Player*>(a);
		if (player != nullptr) {
			color = Colorf(0.42f, 0.35f, 1);
			text.Print(0, 0, fmt::format("({0}) {1}", player->Initiative(), player->Name()), 18, color);
			continue;
		}

		text.Print(0, 0, fmt::format("({0}) {1}", a->Initiative(), a->Name()), 18, color);
	}
}

void InitiativeTracker::ClearInitiatives()
{
	currentTurnIndex = -1;
	initiativeOrder.clear();
	initiativeGroups.clear();
	initiativeNames.clear();
}
