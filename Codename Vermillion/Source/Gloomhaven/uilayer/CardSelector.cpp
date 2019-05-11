#include"pch.h"
#include "CardSelector.h"
#include<algorithm>
#include"GL/glew.h"
#include"../cards/PlayerDeck.h"
#include"../services.h"
#include"../servicelocator.h"
#include<Windows.h>

#include"UILayerId.h"

CardSelect::CardSelect(std::vector<PlayerCard>& cards, Texture& texture, std::function<void(const std::string&)> onclick)
	: texture(texture)
{
	onClick = onclick;

	cardHighlight = -1;
	for (auto& playerCard : cards)
		cardList.push_back(&playerCard);
}

CardSelect::~CardSelect()
{
	cardList.clear();
}

void CardSelect::Resize(const glm::vec2& windowSize)
{
	UILayer::Resize(windowSize);

	scale = size.x / (float)cardList.size();
	scale /= texture.width;

	float maxScale = size.y / texture.height;
	scale = std::min<float>(maxScale, scale);

	totalWidth = cardList.size() * (texture.width * scale);
	centerTranslate = (size.x - totalWidth) / 2.0f;

	position.x = centerTranslate;
	size.x = totalWidth;
}

bool CardSelect::HandleInput(const InputService& input)
{
	if (!UILayer::HandleInput(input)) {
		cardHighlight = -1;
		return false;
	}

	cardHighlight = -1;
	glm::vec2 mousePosition = input.GetMousePosition() - position;

	float x = mousePosition.x / size.x;
	x *= cardList.size();
	cardHighlight = (int)std::floorf(x);

	if (cardHighlight < 0 || cardHighlight > cardList.size())
		cardHighlight = -1;

	if (input.KeyOnce(VK_LBUTTON)) {
		onClick(cardList[cardHighlight]->Name());
	}

	return true;
}

void CardSelect::Render(ServiceLocator& Services)
{
	UILayer::Render(Services);

	glPushMatrix();
	glTranslatef(0, 0, 0.1f);
	glEnable(GL_TEXTURE_2D);

	if (cardHighlight != -1) {
		glPushMatrix();
		glTranslatef(0, -0.75f * texture.height, 0);
		cardList[cardHighlight]->Scale(0.75f);
		cardList[cardHighlight]->Render(Services.Text(), texture);
		glPopMatrix();
	}

	for (auto card : cardList) {
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
