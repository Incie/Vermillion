#pragma once

#include<map>

class Level;

class MonsterIdPool
{
public:
	MonsterIdPool() {
		totalCount = 10;
		idPool.reserve(totalCount);
		for (int i = 1; i <= 10; ++i)
			idPool.push_back(i);
	}

	int Get() {
		if (idPool.size() == 0) {
			return -1;
		}

		int monsterId = idPool.back();
		idPool.pop_back();
		return monsterId;
	}

	int Return(int id) {
		throw "not implemented";
	}

	int totalCount;
	std::vector<int> idPool;
};

class Spawner 
{
public:
	Spawner(Level&level);
	~Spawner();

	void SpawnPlayer(glm::ivec3 location);
	void SpawnMonster(glm::ivec3 location, bool elite);

private:
	int GetMonsterId(const std::string& monsterName);

	std::map<std::string, MonsterIdPool> monsterIdPool;
	Level& level;
	int spawnerIdGenerator;
};