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

	void AddEntitySpawn(const std::string& entityName, const glm::ivec3& location, int roomNumber);
	void SpawnRoom(int roomNumber);

	void SpawnPlayer(glm::ivec3 location);
	void SpawnMonster(glm::ivec3 location, bool elite);
	void SpawnCoinAt(const glm::ivec3& tileLocation);

private:
	struct EntitySpawnLocation {
		std::string entityName;
		glm::ivec3 location;
		int roomNumber;
	};


	void SpawnEntity(const EntitySpawnLocation& esl);
	int GetMonsterId(const std::string& monsterName);

	
	std::vector<EntitySpawnLocation*> entitySpawns;

	std::map<std::string, MonsterIdPool> monsterIdPool;
	Level& level;
	int spawnerIdGenerator;

	struct MonsterStats {
		std::string name;
		int health;
		int range;
		int retaliate;
		int shield;
		int attack;
		int move;
		int team;
		bool elite;
	};

	void SpawnMonster(EntitySpawnLocation& entitySpawnLocation, const MonsterStats& monsterStats);
	std::map<std::string, MonsterStats> monsterStats;
};