#pragma once


#include"uiview.h"
#include<vector>
#include<string>

class Level;
class Texture;
class Tile;

class HoverCard : public UIView {
public:
	HoverCard();
	~HoverCard();

	virtual void Resize(const glm::vec2& windowSize, const TextService& text) override;
	virtual void Measure(const glm::vec2& windowSize, const TextService& text) override;
	virtual void Update(float deltaTime, ServiceLocator& services);
	virtual void Render(ServiceLocator& Services) override;

	void NoEnemyRound();
	void SetEnemyRound(const std::vector<std::string>& enemyRound, int initiative);
	void SetHoverTile(const Level& level, const Tile* tile);
private:
	Texture* entity_icon;
	const Tile* tile;

	Texture* enemyround_texture;
	bool enemyRound;
	int initiative;
	std::vector<std::string> enemyRoundTexts;

	std::vector<std::string> texts;
};