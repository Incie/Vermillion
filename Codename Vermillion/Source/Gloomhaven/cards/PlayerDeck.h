#pragma once

#include<string>
#include<vector>
#include"glm/glm.hpp"
#include"PlayerCard.h"

class Texture;
class TextService;


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
