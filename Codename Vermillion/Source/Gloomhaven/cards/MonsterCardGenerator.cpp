#include"pch.h"
#include "MonsterCardGenerator.h"
#include"MonsterDeck.h"

MonsterCardGenerator::MonsterCardGenerator()
{
}

MonsterCardGenerator::~MonsterCardGenerator()
{
}

std::vector<MonsterDeck*> MonsterCardGenerator::GetMonsterDeck(const std::string& entityName)
{
	auto it = monsterMap.find(entityName);

	if(it == monsterMap.end()) {
		//load from disk

		throw "not implemented";
	}

	auto monsterDeck = monsterMap.find(entityName)->second;
	
	return monsterDeck;
}
