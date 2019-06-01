#pragma once

#include"../uilayer.h"
#include<functional>

class PlayerCard;
class Texture;


//class AbilityCardPanel : public UILayer {
//public:
//
//protected:
//
//};

class AbilitySelector : public UILayer {
public:
	AbilitySelector(Texture& texture, std::function<void(int, int)> callback);
	~AbilitySelector();

	enum {
		ABILITY_TOP,
		ABILITY_BOTTOM,
		ABILITY_DEFAULT_TOP,
		ABILITY_DEFAULT_BOTTOM
	};

	virtual bool HandleInput(const InputService& inputService);
	virtual void Resize(const glm::vec2& windowSize, const TextService& text);
	virtual void Measure(const glm::vec2& dimensions, const TextService& text);
	virtual void Render(ServiceLocator& Services);

	virtual void OnEvent(WindowEvent event, int id);

	void SetTexture(Texture* texture);

	const std::string& GetCardName(int i);
	void SetCards(PlayerCard* card0, PlayerCard* card1);

	virtual UILayerId LayerId() { return UILayerId(); }

	void DisableCard(int cardId);
	void DisableTop();
	void DisableBottom();

protected:
	std::function<void(int, int)> callback;
	float cardScalar;

	Texture* texture;
	PlayerCard *cards[2];
};