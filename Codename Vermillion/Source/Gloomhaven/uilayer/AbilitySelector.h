#pragma once

#include"../uilayer.h"

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
	AbilitySelector(Texture& texture);
	~AbilitySelector();

	virtual bool HandleInput(const InputService& inputService);
	virtual void Resize(const glm::vec2& windowSize);
	virtual void Render(ServiceLocator& Services);

	virtual void OnEvent(WindowEvent event, int id);

	void SetTexture(Texture* texture);
	void SetCards(PlayerCard* card0, PlayerCard* card1);

	virtual UILayerId LayerId() { return UILayerId(); }

	void DisableCard(int cardId);
	void DisableTop();
	void DisableBottom();

protected:
	float cardScalar;

	Texture* texture;
	PlayerCard *cards[2];
};