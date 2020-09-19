#include"pch/pch.h"
#include "PlayerDeck.h"

PlayerDeck::PlayerDeck()
{
}

PlayerDeck::PlayerDeck(const std::string& player, std::vector<PlayerCard>& cards)
	: name(player)
{
	for(auto& card : cards)  {
		handCards.push_back(&card);	
		playingCards.push_back(&card);
	}

	SortHand();
}

PlayerDeck::~PlayerDeck()
{
	handCards.clear();
	playingCards.clear();
	discardedCards.clear();
	lostCards.clear();
	activeCards.clear();
}

const std::vector<PlayerCard*>& PlayerDeck::Active()
{
	return activeCards;
}

const std::vector<PlayerCard*>& PlayerDeck::Hand()
{
	return handCards;
}

const std::vector<PlayerCard*>& PlayerDeck::Discard()
{
	return discardedCards;
}

const std::vector<PlayerCard*>& PlayerDeck::Lost()
{
	return lostCards;
}

void PlayerDeck::Discard(const std::string& cardName)
{
	auto found = std::find_if(handCards.begin(), handCards.end(), [&cardName](auto playerCard) { return (playerCard->Name() == cardName); });
	//auto found = std::find_if(handCards.begin(), handCards.end(), [&cardName](PlayerCard* card) { return (card->Name().compare(cardName) == 0); });

	if(found == handCards.end())
		throw "Trying to discard a non-hand card";

	auto foundCard = *found;
	handCards.erase(found);
	discardedCards.push_back(foundCard);
}

void PlayerDeck::Lose(const std::string& cardName)
{
	auto found = std::find_if(handCards.begin(), handCards.end(), [&cardName](auto playerCard) { return (playerCard->Name() == cardName); });
	//auto found = std::find(handCards.begin(), handCards.end(), [&cardName](PlayerCard* card) { return card->Name() == cardName; });

	if(found == handCards.end())
		throw "Trying to lose a non-hand card";

	auto foundCard = *found;
	lostCards.push_back(foundCard);

	handCards.erase(found);
}

#include <random>
#include <algorithm>
#include <iterator>
#include <numeric>

void PlayerDeck::ShortRest()
{
	if(discardedCards.size() == 0)
		return;

	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_real_distribution<float> dist(0.0f, static_cast<float>(this->discardedCards.size()) );
	auto val = static_cast<int>(dist(mt));

	auto toLost = discardedCards[val];
	discardedCards.erase(discardedCards.begin() + val);
	lostCards.push_back(toLost);

	handCards.insert(handCards.begin(), discardedCards.begin(), discardedCards.end());
	discardedCards.clear();

	SortHand();
}

void PlayerDeck::SortHand()
{
	std::sort(handCards.begin(), handCards.end(), [](auto a, auto b) { return a->Initiative() < b->Initiative(); });
}
