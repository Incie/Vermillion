#include"pch.h"
#include "Spawner.h"
#include"../level/Level.h"
#include"../entity/Entity.h"

#include <random>
#include <algorithm>
#include <iterator>
#include <numeric>

Spawner::Spawner(Level& level)
	: level(level), spawnerIdGenerator(0)
{
}

Spawner::~Spawner()
{
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

	playerattributes pa;
	pa.playerId = 0;
	pa.playerName = "Jazor";
	
	Player* newPlayer = new Player();
	newPlayer->Setup(pa, aa, ea);

	auto& tile = level.TileAt(location);
	newPlayer->SetPosition(tile.Location(), tile.WorldPosition());

	auto hex = Hexagon();
	newPlayer->RenderModel().Generate(glm::vec2(0, 0), 30, 35);
	newPlayer->RenderModel().SetColor(glm::vec3(0.24f, 0.1f, 0.95f));
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

	enemyattributes enemyattr;
	enemyattr.enemyId = GetMonsterId(ea.name);
	enemyattr.enemyType = elite ? EnemyType::Elite : EnemyType::Normal;

	auto* newEnemy = new Enemy();
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
