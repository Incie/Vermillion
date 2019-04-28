#pragma once

#include<string>
#include<vector>
#include"glm/glm.hpp"

class Texture;
class TextService;

class CardAbility
{
public:
	CardAbility(const std::string& name, int value = 0, bool subtitle = false) : name(name), value(value), subtitle(subtitle) {}

	std::string name;
	int value;
	bool subtitle;
};

class PlayerCard
{
public:
	PlayerCard(const std::string& name, std::vector<CardAbility> top, std::vector<CardAbility> bottom, int initiative, int level);
	~PlayerCard();

	void Render(const TextService& text, const Texture& texture);
	void Scale(float scale) { this->scale.x = scale; this->scale.y = scale; }

private:
	void RenderAbility(const TextService& text, const std::vector<CardAbility>& ability, const glm::vec2& position);


	glm::vec2 position;
	glm::vec2 scale;

	std::string name;
	std::vector<CardAbility> topAction;
	std::vector<CardAbility> bottomAction;
	int initiative;
	int level;
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
