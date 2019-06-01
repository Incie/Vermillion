#pragma once

#include"../uilayer.h"
#include<vector>
#include<functional>
#include<string>

class PlayerCard;
class Texture;

class CardSelect : public UILayer {
public:
	CardSelect(std::vector<PlayerCard>& cards, Texture& texture, std::function<void(const std::string&)> onclick);
	virtual ~CardSelect();

	void Resize(const glm::vec2& windowSize, const TextService& text);
	void Measure(const glm::vec2& windowSize, const TextService& text);
	bool HandleInput(const InputService& input);


	void Render(ServiceLocator& Services);

	UILayerId LayerId();

protected:
	Texture& texture;
	std::function<void(const std::string&)> onClick;

	int cardHighlight;

	float centerTranslate;
	float scale;
	float totalWidth;

	std::vector<PlayerCard*> cardList;
};