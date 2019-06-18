#include"pch.h"
#include"MonsterCardGenerator.h"
#include"..//enemyai/EnemyAction.h"
#include"MonsterDeck.h"

MonsterCardGenerator::MonsterCardGenerator(Level& level)
	: level(level)
{
}

MonsterCardGenerator::~MonsterCardGenerator()
{
}

MonsterDeck* MonsterCardGenerator::GetMonsterDeck(const std::string& entityName)
{
	auto it = monsterMap.find(entityName);

	if(it == monsterMap.end()) {
		LoadDeck(entityName);
		return monsterMap[entityName];
	}

	return it->second;
}

void MonsterCardGenerator::DrawAll()
{
	for(auto it : monsterMap) {
		it.second->Draw();
	}
}

void MonsterCardGenerator::Reshuffle()
{
	for(auto it : monsterMap) {
		if(it.second->Active() != nullptr) {
			if( it.second->Active()->Reshuffle() )
				it.second->Shuffle();
		}
	}
}

void MonsterCardGenerator::EndOfRound()
{
	for(auto kv : monsterMap) {
		auto monsterDeck = kv.second;
		
		auto active = monsterDeck->Active();
		if(active != nullptr) {
			if(active->Reshuffle()) {
				monsterDeck->Shuffle();
			}

			monsterDeck->Reset();
		}
	}
}

bool MonsterCardGenerator::LoadDeck(const std::string& entityName)
{
	if(entityName == "Bandit Guard") {
		auto md = vnew MonsterDeck(entityName, std::vector<EnemyRound*>{
			vnew EnemyRound{ 30, {vnew EnemyMove(1), vnew EnemyAttack(-1, 0)} },
			vnew EnemyRound{ 50, {vnew EnemyMove(0), vnew EnemyAttack(0, 0)} },
			vnew EnemyRound{ 15, {vnew EnemyShieldSelf(1), vnew EnemyRetaliate(1) }/*reshuffle*/ },
			vnew EnemyRound{ 70, {vnew EnemyMove(-1), vnew EnemyAttack(1, 0)} },
			vnew EnemyRound{ 35, {vnew EnemyMove(-1), vnew EnemyAttack(0, 1)} },
			vnew EnemyRound{ 15, {vnew EnemyShieldSelf(1), vnew EnemyAttack(0, 0/*poison*/)} },
			vnew EnemyRound{ 50, {vnew EnemyMove(0), vnew EnemyAttack(0, 0)} },
			vnew EnemyRound{ 55, {vnew EnemyMove(-1), vnew EnemyAttack(0, 0)/*strengthen self*/} }
		});
		md->Shuffle();
		monsterMap[entityName] = md;
	}
	else if(entityName == "Living Bones") {
		auto md = vnew MonsterDeck(entityName, std::vector<EnemyRound*>{
			vnew EnemyRound{ 45, {vnew EnemyMove(0), vnew EnemyAttack(0, 0)} },
			vnew EnemyRound{ 12, {vnew EnemyShieldSelf(1), vnew EnemyHealSelf(2) /*reshuffle*/} },
			vnew EnemyRound{ 64, {vnew EnemyMove(-1), vnew EnemyAttack(1, 0)} },
			vnew EnemyRound{ 45, {vnew EnemyMove(0), vnew EnemyAttack(0, 0)} },
			vnew EnemyRound{ 25, {vnew EnemyMove(1), vnew EnemyAttack(-1, 0)} },
			vnew EnemyRound{ 81, {vnew EnemyAttack(2, 0)} },
			vnew EnemyRound{ 74, {vnew EnemyMove(0), vnew EnemyAttack(0, 0 /*3 attacks*/)} },
			vnew EnemyRound{ 20, {vnew EnemyMove(-2), vnew EnemyAttack(0, 0), vnew EnemyHealSelf(2)} /*reshuffle*/ }
		});
		md->Shuffle();
		monsterMap[entityName] = md;
	}
	else if(entityName == "Bandit Archer") {
		auto md = vnew MonsterDeck(entityName, std::vector<EnemyRound*>{
			vnew EnemyRound{ 16, {vnew EnemyMove(1), vnew EnemyAttack(-1, 0)} },
			vnew EnemyRound{ 32, {vnew EnemyMove(0), vnew EnemyAttack(1, -1)} },
			vnew EnemyRound{ 14, {vnew EnemyMove(-1), vnew EnemyAttack(-1, 0)}/* create trap */ },
			vnew EnemyRound{ 56, {vnew EnemyAttack(1, -1/*targets */)} },
			vnew EnemyRound{ 68, {vnew EnemyAttack(1, 1)}/*, reshuffle*/ },
			vnew EnemyRound{ 31, {vnew EnemyMove(0), vnew EnemyAttack(0, 0)} },
			vnew EnemyRound{ 29, {vnew EnemyMove(0), vnew EnemyAttack(-1, 1 /*immobilize*/)}/*, reshuffle*/ },
			vnew EnemyRound{ 44, {vnew EnemyMove(-1), vnew EnemyAttack(1, 0)} }
		});
		md->Shuffle();
		monsterMap[entityName] = md;
	}
	else
		throw "unknown entityName in MonsterCardGenerator";

	return true;
}
