#include"pch.h"
#include "Spawner.h"
#include"../level/Level.h"
#include"../entity/Entity.h"
#include"..//icons/icons.h"

#include <random>
#include <algorithm>
#include <iterator>
#include <numeric>

Spawner::Spawner(Level& level)
	: level(level), spawnerIdGenerator(0)
{
	monsterStats["Bandit Guard"] =		 MonsterStats{ "Bandit Guard",  6, 0, 0, 0, 3, 3, 1, false };
	monsterStats["Bandit Guard Elite"] = MonsterStats{ "Bandit Guard", 10, 0, 0, 1, 4, 2, 1, true };
	monsterStats["Bandit Archer"] =		  MonsterStats{ "Bandit Archer", 6, 4, 0, 0, 2, 3, 1, false };
	monsterStats["Bandit Archer Elite"] = MonsterStats{ "Bandit Archer", 9, 5, 0, 0, 3, 5, 1, true };
	
	monsterStats["Living Bones"]	   = MonsterStats{ "Living Bones", 5, 0, 0, 1, 2, 3, 1, false };
	monsterStats["Living Bones Elite"] = MonsterStats{ "Living Bones", 7, 0, 0, 1, 3, 4, 1, true };
}

Spawner::~Spawner()
{
	for(auto esl : entitySpawns)
		delete esl;
	entitySpawns.clear();
}

void Spawner::AddEntitySpawn(const std::string& entityName, const glm::ivec3& location, int roomNumber)
{
	auto esl = vnew EntitySpawnLocation();
	esl->entityName = entityName;
	esl->location = location;
	esl->roomNumber = roomNumber;
	entitySpawns.push_back(esl);
}

void Spawner::SpawnRoom(int roomNumber)
{
	for(auto esl : entitySpawns) {
		if(roomNumber != esl->roomNumber)
			continue;
		
		if(esl->entityName == "Start") {
			SpawnPlayer(esl->location);
			continue;
		}

		bool isMonster = monsterStats.find(esl->entityName) != monsterStats.end();

		if(isMonster) {
			SpawnMonster(*esl, monsterStats[esl->entityName] );
			continue;
		}

		//isEntity
		SpawnEntity(*esl);
	}
}

void Spawner::SpawnPlayer(glm::ivec3 location)
{
	entityattributes ea;
	ea.entityId = spawnerIdGenerator++;
	ea.name = "PlayerBrute";

	actorattributes aa;
	aa.health = 20;
	aa.range = 0;
	aa.retaliate = 0;
	aa.shield = 0;
	aa.team = 0;
	aa.attack = 0;
	aa.move = 0;
	aa.initiative = 35;
	aa.pierce = 0;

	playerattributes pa;
	pa.playerId = 0;
	pa.playerName = "Jazor";
	
	Player* newPlayer = vnew Player();
	newPlayer->Setup(pa, aa, ea);

	auto& tile = level.TileAt(location);
	newPlayer->SetPosition(tile.Location(), tile.WorldPosition());

	newPlayer->RenderModel().Generate(glm::vec2(0, 0), 30, 35);
	newPlayer->RenderModel().SetColor(glm::vec3(0.24f, 0.1f, 0.95f));
	newPlayer->RenderModel().SetTexture(Icons::Get("brute"));
	tile.SetOccupied(newPlayer->EntityId());
	level.AddEntity(newPlayer);
}

void Spawner::SpawnMonster(glm::ivec3 location, bool elite)
{
	entityattributes ea;
	ea.entityId = spawnerIdGenerator++;
	ea.name = "EnemyBandit";

	actorattributes aa;
	aa.health = elite ? 10 : 6;
	aa.range = elite ? 0 : 0;
	aa.retaliate = elite ? 0 : 0;
	aa.shield = elite ? 1 : 0;
	aa.attack = elite ? 4 : 3;
	aa.move = elite ? 2 : 3;
	aa.team = 1;
	aa.initiative = 50;
	aa.pierce = 0;

	enemyattributes enemyattr;
	enemyattr.enemyId = GetMonsterId(ea.name);
	enemyattr.enemyType = elite ? EnemyType::Elite : EnemyType::Normal;

	auto* newEnemy = vnew Enemy();
	newEnemy->Setup(enemyattr, aa, ea);
	auto& tile = level.TileAt(location);
	newEnemy->SetPosition(tile.Location(), tile.WorldPosition());

	auto& hex = newEnemy->RenderModel();
	hex.Generate(glm::vec2(0, 0), 30, 35);
	if (elite) 
		hex.SetColor(glm::vec3(0.8f, 0.8f, 0.1f));
	else hex.SetColor(glm::vec3(0.8f, 0.8f, 0.8f));
	newEnemy->SetRenderModel(hex);

	tile.SetOccupied(newEnemy->EntityId());
	level.AddEntity(newEnemy);
}

void Spawner::SpawnMonster(EntitySpawnLocation& entitySpawnLocation, const MonsterStats& monsterStats)
{
	entityattributes ea;
	ea.entityId = spawnerIdGenerator++;
	ea.name = monsterStats.name;

	actorattributes aa;
	aa.health = monsterStats.health;
	aa.range = monsterStats.range;
	aa.retaliate = monsterStats.retaliate;
	aa.shield = monsterStats.shield;
	aa.attack = monsterStats.attack;
	aa.move = monsterStats.move;
	aa.team = monsterStats.team;
	aa.pierce = 0;
	aa.initiative = 50;

	enemyattributes enemyattr;
	enemyattr.enemyId = GetMonsterId(ea.name);
	enemyattr.enemyType = monsterStats.elite ? EnemyType::Elite : EnemyType::Normal;

	auto* newEnemy = vnew Enemy();
	newEnemy->Setup(enemyattr, aa, ea);
	auto& tile = level.TileAt(entitySpawnLocation.location);
	newEnemy->SetPosition(tile.Location(), tile.WorldPosition());

	auto& hex = newEnemy->RenderModel();
	hex.Generate(glm::vec2(0, 0), 30, 35);
	hex.SetColor(glm::vec3(1.0f));
	hex.SetTexture(Icons::Get(entitySpawnLocation.entityName));

	tile.SetOccupied(newEnemy->EntityId());
	level.AddEntity(newEnemy);
}

void Spawner::SpawnEntity(const EntitySpawnLocation& esl)
{
	auto& tile = level.TileAt(esl.location);

	entityattributes ea;
	ea.entityId = spawnerIdGenerator++;
	ea.name = esl.entityName;

	auto entity = vnew Entity();
	entity->Setup(ea);
	auto& hex = entity->RenderModel();
	hex.Generate(glm::vec2(tile.WorldPosition().x, tile.WorldPosition().y), 30, 35);
	hex.SetColor(glm::vec3(1.0f));
	hex.SetTexture(Icons::Get(esl.entityName));

	tile.AddEntity(entity);
}

int Spawner::GetMonsterId(const std::string& monsterName)
{
	if (monsterIdPool.find(monsterName) == monsterIdPool.end()) {
		monsterIdPool[monsterName] = MonsterIdPool();
		
		std::random_device rnd;
		std::mt19937_64 randomGenerator(rnd());

		auto& monsterIds = monsterIdPool[monsterName];
		std::shuffle(monsterIds.idPool.begin(), monsterIds.idPool.end(), randomGenerator);
	}
	
	return monsterIdPool[monsterName].Get();
}
