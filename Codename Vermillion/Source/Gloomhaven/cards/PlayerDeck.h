#pragma once

#include<string>
#include<vector>
#include"glm/glm.hpp"
#include"PlayerCard.h"

class Texture;
class TextService;


class PlayerDeck {
public:
	PlayerDeck(const std::string& player, std::vector<PlayerCard>& cards);
	~PlayerDeck();

	const std::vector<PlayerCard*>& Active();
	const std::vector<PlayerCard*>& Hand();
	const std::vector<PlayerCard*>& Discard();
	const std::vector<PlayerCard*>& Lost();

	//void Activate(const std::string& cardName);
	void Discard(const std::string& cardName);
	void Lose(const std::string& cardName);
	
private:
	void SortHand();

	std::string name;

	//put playing cards in a vector here?
	
	std::vector<PlayerCard*> activeCards;
	std::vector<PlayerCard*> discardedCards;
	std::vector<PlayerCard*> lostCards;
	std::vector<PlayerCard*> handCards;

	std::vector<PlayerCard*> playingCards;

private:
	PlayerDeck();
};
