#include"pch.h"
#include "CardSelector.h"
#include<algorithm>
#include"GL/glew.h"
#include"../cards/PlayerDeck.h"
#include"../services.h"
#include"../servicelocator.h"
#include<Windows.h>

#include"UILayerId.h"

CardSelect::CardSelect(PlayerDeck& cards, Texture& texture, std::function<void(const std::string&)> onclick)
	: texture(texture), cardList(&cards)
{
	onClick = onclick;
	cardHighlight = -1;
}

CardSelect::~CardSelect()
{
}

void CardSelect::Resize(const glm::vec2& windowSize, const TextService& text)
{
	if (texture.width == 0 || texture.height == 0)
		return;

	UILayer::Resize(windowSize, text);
}

void CardSelect::Measure(const glm::vec2& windowSize, const TextService& text)
{
	auto& hand = cardList->Hand();

	scale = windowSize.x / (float)hand.size();
	scale /= texture.width;

	float maxScale = 250.0f / texture.height;
	scale = std::min<float>(maxScale, scale);

	totalWidth = hand.size() * (texture.width * scale);

	size.x = totalWidth;
	size.y = scale * texture.height;
}

bool CardSelect::HandleInput(const InputService& input)
{
	if (!UILayer::HandleInput(input)) {
		cardHighlight = -1;
		return false;
	}

	cardHighlight = -1;
	glm::vec2 mousePosition = input.GetMousePosition() - position;


	auto& hand = cardList->Hand();

	float x = mousePosition.x / size.x;
	x *= hand.size();
	cardHighlight = (int)std::floorf(x);

	if (cardHighlight < 0 || cardHighlight > hand.size())
		cardHighlight = -1;

	if (input.KeyOnce(VK_LBUTTON)) {
		onClick(hand[cardHighlight]->Name());
	}

	return true;
}

void CardSelect::Render(ServiceLocator& Services)
{
	UILayer::Render(Services);

	glPushMatrix();
	glTranslatef(0, 0, 0.1f);
	glEnable(GL_TEXTURE_2D);

	auto& hand = cardList->Hand();

	if (cardHighlight != -1) {
		glPushMatrix();
		glTranslatef(0, -0.75f * texture.height, 0);
		hand[cardHighlight]->Scale(0.75f);
		hand[cardHighlight]->Render(Services.Text(), texture);
		glPopMatrix();
	}

	for (auto card : hand) {
		card->Scale(scale);
		card->Render(Services.Text(), texture);
		glTranslatef((texture.width * scale), 0, 0);
	}
	glPopMatrix();
}

UILayerId CardSelect::LayerId()
{
	return UILayerId::CardSelect;
}
