#pragma once

#include"PlayerDeck.h"

class Actor;
class Level;
class PlayerRound;

class CardGenerator
{
public:
	CardGenerator(Level& level);
	~CardGenerator();

	PlayerDeck* PlayerCards();

	PlayerRound* GetTopAction(Actor& actor, const std::string&);
	PlayerRound* GetBottomAction(Actor& actor, const std::string&);
	PlayerRound* GetDefaultTop(Actor& actor);
	PlayerRound* GetDefaultBottom(Actor& actor);

private:
	Level& level;
	std::vector<PlayerCard> playerCards;

	PlayerDeck *playerDeck;
};