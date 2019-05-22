#include"pch.h"
#include "CardSelection.h"
#include"UILayerId.h"

#include"../cards/PlayerDeck.h"
#include"GL/glew.h"

#include"../render.h"

#include"../services.h"
#include"../servicelocator.h"
#include<functional>
#include<Windows.h>

#define BUTTON_ID_READY 1

CardSelection::CardSelection(Texture& texture, std::function<void(CardSelection&, int)> callback)
	: texture(texture), callback(callback)
{
	scalar = 0.66f;
	playerCards[0] = nullptr;
	playerCards[1] = nullptr;

	auto button = new Button();
	button->SetText("Ready");
	button->SetState(UIElement::UIState::DISABLED);
	button->SetColor(glm::vec3(0.6f));
	button->SetTextColor(glm::vec3(0.2f));
	button->SetId(BUTTON_ID_READY);
	children.push_back(button);
}

CardSelection::~CardSelection()
{
}

void CardSelection::Resize(const glm::vec2& windowSize)
{
	UILayer::Resize(windowSize);

	float width = scalar * texture.width * 2 + 3 * 8.0f;
	float mainHeight = scalar * texture.height + 3 * 8.0f + 22.0f;
	float height = mainHeight + 8 + 8 + 22.0f; //button height + margins

	size.x = width;
	size.y = height;

	position.x = (windowSize.x - size.x);
	position.y = (windowSize.y - size.y) / 2.0f;

	auto button = dynamic_cast<Button*>(children[0]);
	button->SetPosition(width * 0.5f - 125.f * 0.5f, mainHeight + 8);
	button->SetSize(125.0f, 22.0f);
	button->SetTextSize(18.f);
}

bool CardSelection::HandleInput(const InputService& input)
{
	if (!UILayer::HandleInput(input)) {
		return false;
	}

	glm::vec2 mousePosition = input.GetMousePosition() - position;


	if (input.KeyOnce(VK_LBUTTON))
	{
		if (playerCards[0] != nullptr) {
			if (playerCards[0]->PointInside(mousePosition - glm::vec2(8, 16 + 22), scalar, texture))
				playerCards[0] = nullptr;
		}

		if (playerCards[1] != nullptr) {
			if (playerCards[1]->PointInside(mousePosition - glm::vec2(8 + 8 + texture.width * scalar, 16 + 22), scalar, texture))
				playerCards[1] = nullptr;
		}


		children[0]->SetState(UIElement::UIState::DISABLED);
		if (playerCards[1] != nullptr && playerCards[0] != nullptr) {
			children[0]->SetState(UIElement::UIState::ENABLED);
		}
	}

	return true;
}

void CardSelection::Render(ServiceLocator& Services)
{
	UILayer::Render(Services);

	Services.Text().PrintCenter(8 + 0.5 * (double)scalar * (double)texture.width, 8.0 + 11.0, "Initiative", 22.0f, Colorf(0));

	float w = texture.width;
	float h = texture.height;

	glPushMatrix();
	glTranslatef(8, 8 + 8 + 22.0f, 0);
	Render::Quad(0, 0, scalar * texture.width, scalar * texture.height, glm::vec3(0.88));
	if (playerCards[0] != nullptr) {
		playerCards[0]->Scale(scalar);
		playerCards[0]->Render(Services.Text(), texture);
	}

	glTranslatef(8 + texture.width * scalar, 0, 0);
	Render::Quad(0, 0, (double)scalar * (double)texture.width, (double)scalar * (double)texture.height, glm::vec3(0.88f));
	if (playerCards[1] != nullptr) {
		playerCards[1]->Scale(scalar);
		playerCards[1]->Render(Services.Text(), texture);
	}
	glPopMatrix();
}

void CardSelection::AddCard(const PlayerCard& playerCard)
{
	if (CardExists(playerCard))
		return;

	if (playerCards[0] == nullptr )
		playerCards[0] = &playerCard;
	else if (playerCards[1] == nullptr) 
		playerCards[1] = &playerCard;

	children[0]->SetState(UIElement::UIState::DISABLED);
	if (playerCards[1] != nullptr && playerCards[0] != nullptr) {
		children[0]->SetState(UIElement::UIState::ENABLED);
	}
}

bool CardSelection::IsFull() const
{
	return false;
}

UILayerId CardSelection::LayerId()
{
	return UILayerId::CardSelection;
}

void CardSelection::OnEvent(WindowEvent type, int id)
{
	callback(*this, 0);
}

const std::string& CardSelection::Card(int i)
{
	return playerCards[i]->Name();
}

bool CardSelection::CardExists(const PlayerCard& card) const
{
	for( int i = 0; i < 2; ++i ){
		if (playerCards[i] != nullptr && playerCards[i]->Name().compare(card.Name()) != 0)
			return true;
	}
	return false;
}
