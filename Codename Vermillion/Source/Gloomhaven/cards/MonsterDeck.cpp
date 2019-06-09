#include"pch.h"
#include "MonsterDeck.h"

#include <random>
#include <algorithm>
#include <iterator>
#include <numeric>

MonsterDeck::MonsterDeck(const std::string& entityName)
	: entityName(entityName), active(nullptr)
{

}

MonsterDeck::~MonsterDeck()
{
	active = nullptr;
	enemyRounds.clear();
	discardedRounds.clear();
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
