#pragma once

#include"ui/uiview.h"
#include<vector>
#include<functional>
#include<string>

class PlayerDeck;
class PlayerCard;
class Texture;

class CardSelect : public UIView {
public:
	CardSelect(PlayerDeck& cards, Texture& texture, std::function<void(const std::string&)> onclick);
	virtual ~CardSelect();

	void Resize(const glm::vec2& windowSize, const TextService& text);
	void Measure(const glm::vec2& windowSize, const TextService& text);
	bool HandleInput(const InputService& input);


	void Render(ServiceLocator& Services);

protected:
	Texture& texture;
	std::function<void(const std::string&)> onClick;

	int cardHighlight;

	float centerTranslate;
	float scale;
	float totalWidth;

	PlayerDeck* cardList;
};