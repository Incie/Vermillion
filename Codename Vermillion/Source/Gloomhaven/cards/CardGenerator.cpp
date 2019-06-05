#include"pch.h"
#include"CardGenerator.h"

#include"../action/Action.h"
#include"../action/ActionAttack.h"
#include"../action/ActionMove.h"
#include"../action/ActionShieldSelf.h"

#include"PlayerRound.h"
#include"../level/Level.h"

#define ATTACK(r, d, a) vnew ActionAttack(level, actor, r, d, a)
#define MOVE(m) vnew ActionMove(level, actor, m)
#define SHIELD(s) vnew ActionShieldSelf(level, actor, s)

CardGenerator::CardGenerator(Level& level)
	: level(level)
{
	playerCards = std::vector<PlayerCard>{
		PlayerCard{
			"Immovable Phalanx",
			{CardAbility{"Attack", 4}, CardAbility{"Shield", 1}, CardAbility{"self", 0, true}},
			{CardAbility{"Move", 4}}, 
			17,	6,
			[](Level& level, Actor& actor) {auto v = std::vector<Action*>{vnew ActionAttack(level, actor, 1, 4, 1), vnew ActionShieldSelf(level, actor, 1)};  return vnew PlayerRound(v); },
			[](Level& level, Actor& actor) {auto v = std::vector<Action*>{MOVE(4)};  return vnew PlayerRound(v); }
		},
		PlayerCard{
			"Skirmishing Manouver",
			{CardAbility{"Attack", 2}, CardAbility{"Move", 2},CardAbility{"Attack", 2}},
			{CardAbility{"Attack", 3}, CardAbility{"Range", 3, true}, CardAbility{"XP", 1, true}}, 
			29,	5,
			[](Level& level, Actor& actor) {auto v = std::vector<Action*>{ATTACK(1, 2, 1), MOVE(2), ATTACK(1,2,1)};  return vnew PlayerRound(v); },
			[](Level& level, Actor& actor) {auto v = std::vector<Action*>{ATTACK(3, 3, 1)};  return vnew PlayerRound(v); }
		},
		PlayerCard{
			"Unstoppable Charge",
			{CardAbility{"Attack", 5}, CardAbility{"XP", 1}},
			{CardAbility{"Move", 4}, CardAbility{"STUN"}, CardAbility{"All adjacent enemies"}, CardAbility{"LOSS"}},
			86, 4,
			[](Level& level, Actor& actor) {auto v = std::vector<Action*>{ATTACK(1,5,1)};  return vnew PlayerRound(v); },
			[](Level& level, Actor& actor) {auto v = std::vector<Action*>{MOVE(4)};  return vnew PlayerRound(v); }
		},
		PlayerCard{
			"Juggernaut",
			{CardAbility{"Move", 2}, CardAbility{"Attack", 2}},
			{CardAbility{"Move", 4}},
			34, 2,
			[](Level& level, Actor& actor) {auto v = std::vector<Action*>{MOVE(2), ATTACK(1,2,1)};  return vnew PlayerRound(v); },
			[](Level& level, Actor& actor) {auto v = std::vector<Action*>{MOVE(4)};  return vnew PlayerRound(v); }
		},
		PlayerCard{
			"Hook and Chain",
			{CardAbility{"Attack", 3}, CardAbility{"Range", 3, true}, CardAbility{"PULL", 2, true}},
			{CardAbility{"Move", 4}},
			42, 3,
			[](Level& level, Actor& actor) {auto v = std::vector<Action*>{ATTACK(2,3,1)};  return vnew PlayerRound(v); },
			[](Level& level, Actor& actor) {auto v = std::vector<Action*>{MOVE(4)};  return vnew PlayerRound(v); }
		},

		PlayerCard{
			"Leaping Cleave",
			{CardAbility{"Attack", 3}, CardAbility{"Target", 2, true}, CardAbility{"XP", 1}},
			{CardAbility{"Move", 3}, CardAbility{"Jump"}},
			54, 1,
			[](Level& level, Actor& actor) {auto v = std::vector<Action*>{ATTACK(1,3,2)};  return vnew PlayerRound(v); },
			[](Level& level, Actor& actor) {auto v = std::vector<Action*>{MOVE(3)};  return vnew PlayerRound(v); }
		},
		PlayerCard{
			"Trample",
			{CardAbility{"Attack", 3}, CardAbility{"Pierce", 2, true}},
			{CardAbility{"Move", 4}, CardAbility{"Jump", 0, true}, CardAbility{"Attack", 2}, CardAbility{"Target all enemies moved through",0, true}, CardAbility{"XP", 2}, CardAbility{"LOSS"}},
			72, 1,
			[](Level& level, Actor& actor) {auto v = std::vector<Action*>{ATTACK(1,3,1)};  return vnew PlayerRound(v); },
			[](Level& level, Actor& actor) {auto v = std::vector<Action*>{vnew ActionTrample(level,actor,4,true,2)}; return vnew PlayerRound(v); }
		},
		PlayerCard{
			"Skewer",
			{CardAbility{"Attack", 3}, CardAbility{"Range", 3, true}, CardAbility{"PULL", 2, true}},
			{CardAbility{"Move", 4}},
			35, 1,
			[](Level& level, Actor& actor) {auto v = std::vector<Action*>{ATTACK(3,3,1)};  return vnew PlayerRound(v); },
			[](Level& level, Actor& actor) {auto v = std::vector<Action*>{MOVE(4)};  return vnew PlayerRound(v); }
		},
		PlayerCard{
			"Spare Dagger",
			{CardAbility{"Attack", 3}, CardAbility{"Range", 3, true}, CardAbility{"XP", 1}},
			{CardAbility{"Attack", 2}},
			27, 1,
			[](Level& level, Actor& actor) {auto v = std::vector<Action*>{ATTACK(3,3,1)};  return vnew PlayerRound(v); },
			[](Level& level, Actor& actor) {auto v = std::vector<Action*>{ATTACK(1,2,1)};  return vnew PlayerRound(v); }
		},
		PlayerCard{
			"Provoking Roar",
			{CardAbility{"Attack", 2}, CardAbility{"DISARM", 0, true}},
			{CardAbility{"Move", 2}},
			10, 1,
			[](Level& level, Actor& actor) {auto v = std::vector<Action*>{ATTACK(1,2,1)};  return vnew PlayerRound(v); },
			[](Level& level, Actor& actor) {auto v = std::vector<Action*>{MOVE(2)};  return vnew PlayerRound(v); }
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

#include<algorithm>
PlayerRound* CardGenerator::GetTopAction(Actor& actor, const std::string& cardName)
{
	auto card = std::find_if(playerCards.begin(), playerCards.end(), [&cardName](auto playerCard) { if (playerCard.Name().compare(cardName) == 0) return true; return false; });

	if (card == playerCards.end())
		throw "Card Not Found";

	return (*card).GenerateTopAction(level, actor);
}

PlayerRound* CardGenerator::GetBottomAction(Actor& actor, const std::string& cardName)
{
	auto card = std::find_if(playerCards.begin(), playerCards.end(), [&cardName](auto playerCard) { if (playerCard.Name().compare(cardName) == 0) return true; return false; });

	if (card == playerCards.end())
		throw "Card Not Found";

	return (*card).GenerateBottomAction(level, actor);
}

PlayerRound* CardGenerator::GetDefaultTop(Actor& actor)
{
	auto vec = std::vector<Action*>{ vnew ActionAttack(level, actor, 1, 2, 1) };
	return vnew PlayerRound(vec);
}

PlayerRound* CardGenerator::GetDefaultBottom(Actor& actor)
{
	auto vec = std::vector<Action*>{ vnew ActionMove(level, actor, 2) };
	return vnew PlayerRound(vec);
}
