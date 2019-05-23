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
	void Measure(const glm::vec2& dimensions);
	bool IsFull() const;
	UILayerId LayerId();

	void OnEvent(WindowEvent type, int id);

	const std::string& Card(int i);
	void ClearCards() { playerCards[0] = nullptr; playerCards[1] = nullptr; }

protected:
	bool CardExists(const PlayerCard& card) const;

	std::function<void(CardSelection&, int)> callback;
	Texture& texture;
	PlayerCard const* playerCards[2];
	float scalar;
};