#include"pch.h"
#include"CardGenerator.h"

#include"../action/Action.h"
#include"../action/ActionAttack.h"
#include"../action/ActionMove.h"

#include"PlayerRound.h"
#include"../level/Level.h"

CardGenerator::CardGenerator(Level& level)
	: level(level)
{
	playerCards = std::vector<PlayerCard>{
		PlayerCard{
			"Immovable Phalanx",
			{CardAbility{"Attack", 4}, CardAbility{"Shield", 1}, CardAbility{"self", 0, true}},
			{CardAbility{"Move", 4}}, 
			17,	6
		},
		PlayerCard{
			"Skirmishing Manouver",
			{CardAbility{"Attack", 2}, CardAbility{"Move", 2},CardAbility{"Attack", 2}},
			{CardAbility{"Attack", 3}, CardAbility{"Range", 3, true}, CardAbility{"XP", 1, true}}, 
			29,	5
		},
		PlayerCard{
			"Unstoppable Charge",
			{CardAbility{"Attack", 5}, CardAbility{"XP", 1}},
			{CardAbility{"Move", 4}, CardAbility{"STUN"}, CardAbility{"All adjacent enemies"}, CardAbility{"LOSS"}},
			86, 4
		},
		PlayerCard{
			"Juggernaut",
			{CardAbility{"Move", 2}, CardAbility{"Attack", 2}},
			{CardAbility{"Move", 4}},
			34, 2
		},
		PlayerCard{
			"Hook and Chain",
			{CardAbility{"Attack", 3}, CardAbility{"Range", 3, true}, CardAbility{"PULL", 2, true}},
			{CardAbility{"Move", 4}},
			42, 3
		},

		PlayerCard{
			"Leaping Cleave",
			{CardAbility{"Attack", 3}, CardAbility{"Target", 2, true}, CardAbility{"XP", 1}},
			{CardAbility{"Move", 3}, CardAbility{"Jump"}},
			54, 1
		},
		PlayerCard{
			"Trample",
			{CardAbility{"Attack", 3}, CardAbility{"Pierce", 2, true}},
			{CardAbility{"Move", 4}, CardAbility{"Jump", 0, true}, CardAbility{"Attack", 2}, CardAbility{"Target all enemies moved through",0, true}, CardAbility{"XP", 2}, CardAbility{"LOSS"}},
			72, 1
		},
		PlayerCard{
			"Skewer",
			{CardAbility{"Attack", 3}, CardAbility{"Range", 3, true}, CardAbility{"PULL", 2, true}},
			{CardAbility{"Move", 4}},
			35, 1
		},
		PlayerCard{
			"Spare Dagger",
			{CardAbility{"Attack", 3}, CardAbility{"Range", 3, true}, CardAbility{"XP", 1}},
			{CardAbility{"Attack", 2}},
			27, 1
		},
		PlayerCard{
			"Provoking Roar",
			{CardAbility{"Attack", 2}, CardAbility{"DISARM", 0, true}},
			{CardAbility{"Move", 2}},
			10, 1
		},
	};
}

CardGenerator::~CardGenerator()
{
	playerCards.clear();
}

std::vector<PlayerCard>& CardGenerator::PlayerCards()
{
	return playerCards;
}

PlayerRound* CardGenerator::GetTopAction(Actor& actor, const std::string&)
{
	return nullptr;
}

PlayerRound* CardGenerator::GetBottomAction(Actor& actor, const std::string&)
{
	return nullptr;
}

PlayerRound* CardGenerator::GetDefaultTop(Actor& actor)
{
	auto vec = std::vector<Action*>{ new ActionAttack(level, actor, 1, 2, 1) };
	return new PlayerRound(vec);
}

PlayerRound* CardGenerator::GetDefaultBottom(Actor& actor)
{
	auto vec = std::vector<Action*>{ new ActionMove(level, actor, 2) };
	return new PlayerRound(vec);
}
