#pragma once

#include"uiview.h"
#include"glm/glm.hpp"
#include<functional>
#include<string>

class Texture;
class PlayerCard;

class CardSelection : public UIView {
public:
	CardSelection(Texture& texture, std::function<void(CardSelection&, int)> callback);
	virtual ~CardSelection();

	void Resize(const glm::vec2& windowSize, const TextService& text);
	void Measure(const glm::vec2& dimensions, const TextService& text);

	bool HandleInput(const InputService& input);
	void Render(ServiceLocator& Services);
	void AddCard(const PlayerCard& playerCard);
	bool IsFull() const;

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