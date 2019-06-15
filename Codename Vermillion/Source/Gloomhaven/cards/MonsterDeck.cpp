#include"pch.h"
#include "MonsterDeck.h"

#include <random>
#include <algorithm>
#include <iterator>
#include <numeric>

MonsterDeck::MonsterDeck(const std::string& entityName, std::vector<EnemyRound*> er)
	: entityName(entityName), active(nullptr)
{
	enemyRounds.insert(enemyRounds.end(), er.begin(), er.end());
}

MonsterDeck::~MonsterDeck()
{
	active = nullptr;
	enemyRounds.clear();
	discardedRounds.clear();
}

EnemyRound* MonsterDeck::Active() {
	return active;
}

void MonsterDeck::Draw()
{
	active = enemyRounds.back();
	discardedRounds.push_back(active);
	enemyRounds.pop_back();
}

void MonsterDeck::Shuffle()
{
	enemyRounds.insert(enemyRounds.end(), discardedRounds.begin(), discardedRounds.end());
	discardedRounds.clear();

	std::random_device rnd;
	std::mt19937_64 gen(rnd());

	std::shuffle(enemyRounds.begin(), enemyRounds.end(), gen);
}
