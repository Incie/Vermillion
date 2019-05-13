#pragma once

#include"PlayerDeck.h"


class CardGenerator
{
public:
	CardGenerator();
	~CardGenerator();

	std::vector<PlayerCard>& PlayerCards();



private:

	std::vector<PlayerCard> playerCards;

};