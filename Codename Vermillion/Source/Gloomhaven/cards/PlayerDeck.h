#pragma once

#include<string>
#include<vector>


class PlayerCard {
public:

private:
	bool isLost;

	//list of top actions
	//list of bottom actions

};


class PlayerDeck {
public:
	PlayerDeck();
	~PlayerDeck();



private:
	std::vector<PlayerCard*> activeCards;
	std::vector<PlayerCard*> discardedCards;
	std::vector<PlayerCard*> lostCards;
	std::vector<PlayerCard*> handCards;

	std::vector<PlayerCard*> playingCards;
};
