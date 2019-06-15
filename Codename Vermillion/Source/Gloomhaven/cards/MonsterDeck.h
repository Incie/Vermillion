#pragma once


#include<vector>
#include"../enemyai/EnemyRound.h"

class MonsterDeck
{
public:
	MonsterDeck(const std::string& entityName, std::vector<EnemyRound*> er);
	~MonsterDeck();

	void Draw();
	void Shuffle();

	EnemyRound* Active();

private:
	std::string entityName;

	EnemyRound* active;

	std::vector<EnemyRound*> enemyRounds;
	std::vector<EnemyRound*> discardedRounds;
};