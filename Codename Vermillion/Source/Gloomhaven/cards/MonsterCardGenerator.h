#pragma once

#include<string>
#include<vector>
#include<map>

class MonsterDeck;

class MonsterCardGenerator
{
public:
	MonsterCardGenerator();
	~MonsterCardGenerator();

	std::vector<MonsterDeck*> GetMonsterDeck(const std::string& entityName);


private:
	std::map<std::string, std::vector<MonsterDeck*>> monsterMap;
};