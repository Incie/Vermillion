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
		//load from disk

		throw "not implemented";
	}

	auto monsterDeck = it->second;
	
	return monsterDeck;
}

bool MonsterCardGenerator::LoadDeck(const std::string& entityName)
{
	if(entityName == "Bandit Guard") {
		auto md = new MonsterDeck(entityName, std::vector<EnemyRound*>{
			new EnemyRound{ 30, {new EnemyMove(level, 1), new EnemyAttack(level, -1, 0)} },
				new EnemyRound{ 50, {new EnemyMove(level, 0), new EnemyAttack(level, 0, 0)} },
				new EnemyRound{ 15, {/*Shield1, Retaliate 2*/}/*reshuffle*/ },
				new EnemyRound{ 70, {new EnemyMove(level, -1), new EnemyAttack(level, 1, 0)} },
				new EnemyRound{ 35, {new EnemyMove(level, -1), new EnemyAttack(level, 0, 1)} },
				new EnemyRound{ 15, {/*shield 1*/ new EnemyAttack(level, 0, 1/*poison*/)} },
				new EnemyRound{ 50, {new EnemyMove(level, 0), new EnemyAttack(level, 0, 0)} },
				new EnemyRound{ 55, {new EnemyMove(level, -1), new EnemyAttack(level, 0, 0)/*strengthen self*/} },

		}
	}
	else if(entityName == "Living Bones") {
		auto md = new MonsterDeck(entityName, std::vector<EnemyRound*>{
			new EnemyRound{ 45, {new EnemyMove(level, 0), new EnemyAttack(level, 0, 0)} },
			//new EnemyRound{ 12, {/*new EnemyShield(level, 0),*/ /*new EnemyHeal(level, 2),*//*reshuffle*/ },
			new EnemyRound{ 64, {new EnemyMove(level, -1), new EnemyAttack(level, 1, 0)} },
			new EnemyRound{ 45, {new EnemyMove(level, 0), new EnemyAttack(level, 0, 0)} },
			new EnemyRound{ 25, {new EnemyMove(level, 1), new EnemyAttack(level, -1, 0)} },
			new EnemyRound{ 81, {new EnemyAttack(level, 2, 0)} },
			new EnemyRound{ 74, {new EnemyMove(level, 0), new EnemyAttack(level, 0, 0 /*3 attacks*/)} },
			new EnemyRound{ 20, {new EnemyMove(level, -2), new EnemyAttack(level, 0, 0)/* healf self 2*/} /*reshuffle*/ }
		});
	}
	else if(entityName == "Bandit Archer") {
		auto md = new MonsterDeck(entityName, std::vector<EnemyRound*>{
			new EnemyRound{ 16, {new EnemyMove(level, 1), new EnemyAttack(level, -1, 0)} },
			new EnemyRound{ 32, {new EnemyMove(level, 0), new EnemyAttack(level, 1, -1)} },
			new EnemyRound{ 14, {new EnemyMove(level, -1), new EnemyAttack(level, -1, 0)}/* create trap */ },
			new EnemyRound{ 56, {new EnemyAttack(level, 1, -1/*targets */)} },
			new EnemyRound{ 68, {new EnemyAttack(level, 1, 1)}/*, reshuffle*/  },
			new EnemyRound{ 31, {new EnemyMove(level, 0), new EnemyAttack(level, 0, 0)} },
			new EnemyRound{ 29, {new EnemyMove(level, 0), new EnemyAttack(level, -1, 1 /*immobilize*/)}/*, reshuffle*/ },
			new EnemyRound{ 44, {new EnemyMove(level, -1), new EnemyAttack(level, 1, 0)} }
		});
	}
}
