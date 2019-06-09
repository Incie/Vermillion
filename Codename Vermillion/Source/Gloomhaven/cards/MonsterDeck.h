#pragma once


#include<vector>
#include"../enemyai/EnemyRound.h"

class MonsterDeck
{
public:
	MonsterDeck(const std::string& entityName);
	~MonsterDeck();

	void Draw();

	void Shuffle();

private:
	std::string entityName;

	EnemyRound* active;

	std::vector<EnemyRound*> enemyRounds;
	std::vector<EnemyRound*> discardedRounds;

};