#pragma once

#include<string>
#include<vector>
#include<map>

class Level;
class MonsterDeck;

class MonsterCardGenerator
{
public:
	MonsterCardGenerator(Level& level);
	~MonsterCardGenerator();

	MonsterDeck* GetMonsterDeck(const std::string& entityName);

private:
	bool LoadDeck(const std::string& entityName);

	Level& level;
	std::map<std::string, MonsterDeck*> monsterMap;
};