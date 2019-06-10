#pragma once


#include"..//uiview.h"
#include<vector>
#include<string>

class Level;
class Texture;
class Tile;

class HoverCard : public UIView {
public:
	HoverCard();
	~HoverCard();

	virtual void Render(ServiceLocator& Services) override;
	virtual void Resize(const glm::vec2& windowSize, const TextService& text) override;
	virtual void Measure(const glm::vec2& windowSize, const TextService& text) override;

	void SetHoverTile(const Level& level, const Tile* tile);
private:
	Texture* entity_icon;
	const Tile* tile;

	std::vector<std::string> texts;
};