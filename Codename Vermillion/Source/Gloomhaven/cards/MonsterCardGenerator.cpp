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
		//if( it.second->NeedsReshuffle() ) 
		it.second->Shuffle();
	}
}

bool MonsterCardGenerator::LoadDeck(const std::string& entityName)
{
	if(entityName == "Bandit Guard") {
		auto md = vnew MonsterDeck(entityName, std::vector<EnemyRound*>{
			vnew EnemyRound{ 30, {vnew EnemyMove(level, 1), vnew EnemyAttack(level, -1, 0)} },
			vnew EnemyRound{ 50, {vnew EnemyMove(level, 0), vnew EnemyAttack(level, 0, 0)} },
			vnew EnemyRound{ 15, {vnew EnemyShieldSelf(level, 1), vnew EnemyRetaliate(level, 1) }/*reshuffle*/ },
			vnew EnemyRound{ 70, {vnew EnemyMove(level, -1), vnew EnemyAttack(level, 1, 0)} },
			vnew EnemyRound{ 35, {vnew EnemyMove(level, -1), vnew EnemyAttack(level, 0, 1)} },
			vnew EnemyRound{ 15, {vnew EnemyShieldSelf(level, 1), vnew EnemyAttack(level, 0, 0/*poison*/)} },
			vnew EnemyRound{ 50, {vnew EnemyMove(level, 0), vnew EnemyAttack(level, 0, 0)} },
			vnew EnemyRound{ 55, {vnew EnemyMove(level, -1), vnew EnemyAttack(level, 0, 0)/*strengthen self*/} }
		});
		md->Shuffle();
		monsterMap[entityName] = md;
	}
	else if(entityName == "Living Bones") {
		auto md = vnew MonsterDeck(entityName, std::vector<EnemyRound*>{
			vnew EnemyRound{ 45, {vnew EnemyMove(level, 0), vnew EnemyAttack(level, 0, 0)} },
			vnew EnemyRound{ 12, {vnew EnemyShieldSelf(level, 1), vnew EnemyHealSelf(level, 2) /*reshuffle*/} },
			vnew EnemyRound{ 64, {vnew EnemyMove(level, -1), vnew EnemyAttack(level, 1, 0)} },
			vnew EnemyRound{ 45, {vnew EnemyMove(level, 0), vnew EnemyAttack(level, 0, 0)} },
			vnew EnemyRound{ 25, {vnew EnemyMove(level, 1), vnew EnemyAttack(level, -1, 0)} },
			vnew EnemyRound{ 81, {vnew EnemyAttack(level, 2, 0)} },
			vnew EnemyRound{ 74, {vnew EnemyMove(level, 0), vnew EnemyAttack(level, 0, 0 /*3 attacks*/)} },
			vnew EnemyRound{ 20, {vnew EnemyMove(level, -2), vnew EnemyAttack(level, 0, 0), vnew EnemyHealSelf(level, 2)} /*reshuffle*/ }
		});
		md->Shuffle();
		monsterMap[entityName] = md;
	}
	else if(entityName == "Bandit Archer") {
		auto md = vnew MonsterDeck(entityName, std::vector<EnemyRound*>{
			vnew EnemyRound{ 16, {vnew EnemyMove(level, 1), vnew EnemyAttack(level, -1, 0)} },
			vnew EnemyRound{ 32, {vnew EnemyMove(level, 0), vnew EnemyAttack(level, 1, -1)} },
			vnew EnemyRound{ 14, {vnew EnemyMove(level, -1), vnew EnemyAttack(level, -1, 0)}/* create trap */ },
			vnew EnemyRound{ 56, {vnew EnemyAttack(level, 1, -1/*targets */)} },
			vnew EnemyRound{ 68, {vnew EnemyAttack(level, 1, 1)}/*, reshuffle*/ },
			vnew EnemyRound{ 31, {vnew EnemyMove(level, 0), vnew EnemyAttack(level, 0, 0)} },
			vnew EnemyRound{ 29, {vnew EnemyMove(level, 0), vnew EnemyAttack(level, -1, 1 /*immobilize*/)}/*, reshuffle*/ },
			vnew EnemyRound{ 44, {vnew EnemyMove(level, -1), vnew EnemyAttack(level, 1, 0)} }
		});
		md->Shuffle();
		monsterMap[entityName] = md;
	}
	else
		throw "unknown entityName in MonsterCardGenerator";

	return true;
}
