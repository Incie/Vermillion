#pragma once

#include"../uilayer.h"
#include"glm/glm.hpp"
#include<functional>
#include<string>

class Texture;
class PlayerCard;

class CardSelection : public UILayer {
public:
	CardSelection(Texture& texture, std::function<void(CardSelection&, int)> callback);
	virtual ~CardSelection();

	void Resize(const glm::vec2& windowSize);
	bool HandleInput(const InputService& input);
	void Render(ServiceLocator& Services);
	void AddCard(const PlayerCard& playerCard);
	bool IsFull() const;
	UILayerId LayerId();

	void OnEvent(WindowEvent type, int id);

	const std::string& Card(int i);

protected:
	std::function<void(CardSelection&, int)> callback;
	Texture& texture;
	PlayerCard const* playerCards[2];
	float scalar;
};