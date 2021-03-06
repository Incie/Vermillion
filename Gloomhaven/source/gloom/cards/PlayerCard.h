#pragma once

#include<string>
#include<vector>
#include<functional>
#include"glm/glm.hpp"

class Texture;
class TextService;
class PlayerRound;
class Actor;
class Level;

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
	PlayerCard(const std::string& name, std::vector<CardAbility> top, std::vector<CardAbility> bottom, int initiative, int level, std::function<PlayerRound*(Level&, Actor&)> generatorTop, std::function<PlayerRound* (Level&, Actor&)> generatorBottom);
	~PlayerCard();

	void Render(const TextService& text, const Texture& texture) const;
	void Scale(float scale) const { this->scale.x = scale; this->scale.y = scale; }

	const std::string& Name() const  { return name; }
	int Initiative() const { return initiative; }

	bool PointInside(const glm::vec2& point, double scalar, const Texture& texture) const;

	PlayerRound* GenerateTopAction(Level& level, Actor& actor) const;
	PlayerRound* GenerateBottomAction(Level& level, Actor& actor) const;
private:
	void RenderAbility(const TextService& text, const std::vector<CardAbility>& ability, const glm::vec2& position) const;

	std::function<PlayerRound* (Level&, Actor&)> generatorTop;
	std::function<PlayerRound* (Level&, Actor&)> generatorBottom;

	glm::vec2 position;
	mutable glm::vec2 scale;

	std::string name;
	std::vector<CardAbility> topAction;
	std::vector<CardAbility> bottomAction;
	int initiative;
	int level;
};
