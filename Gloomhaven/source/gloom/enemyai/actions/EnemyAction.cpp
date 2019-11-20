#include"pch/pch.h"
#include"EnemyAction.h"


EnemyAction::EnemyAction()
	: state(0), calculated(false), startPoint(glm::vec2())
{
}

EnemyAction::~EnemyAction()
{
	targets.clear();
}



//=========


EnemyCreateTrap::EnemyCreateTrap(int trapDamage, std::vector<StatusEffect> statusEffects)
	: trapDamage(trapDamage), statusEffects(statusEffects)
{
}

void EnemyCreateTrap::Perform(Director& director, Level& level, Actor& actor)
{
}

void EnemyCreateTrap::Calculate(Level& level, const Actor& actor)
{
}

void EnemyCreateTrap::Render()
{
}
