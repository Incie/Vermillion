#include"pch/pch.h"
#include "CardSelection.h"
#include"../cards/PlayerDeck.h"
#include"GL/glew.h"

#include"render.h"

#include"services.h"
#include"servicelocator.h"
#include<functional>
#include<Windows.h>

#define ButtonIdReady 1
#define ButtonIdShortRest 2
#define ButtonIdLongRest 3

CardSelection::CardSelection(Texture& texture, std::function<void(CardSelection&, int)> callback)
	: texture(texture), callback(callback)
{
	SetAnchor(UIView::WindowAnchor::RIGHT | UIView::WindowAnchor::TOP | UIView::WindowAnchor::BOTTOM);

	scalar = 0.66f;
	playerCards[0] = nullptr;
	playerCards[1] = nullptr;

	auto button = vnew Button();
	button->SetText("Ready");
	button->SetState(UIElement::UIState::DISABLED);
	button->SetColor(glm::vec3(0.6f));
	button->SetTextColor(glm::vec3(0.2f));
	button->SetId(ButtonIdReady);
	button->SetSize(125.0f, 22.0f);
	button->SetTextSize(18.f);
	children.push_back(button);

	auto buttonShortRest = vnew Button();
	buttonShortRest->SetText("Short Rest");
	buttonShortRest->SetColor(glm::vec3(0.6f));
	buttonShortRest->SetTextColor(glm::vec3(0.2f));
	buttonShortRest->SetId(ButtonIdShortRest);
	buttonShortRest->SetSize(125.0f, 22.0f);
	buttonShortRest->SetTextSize(18.f);
	children.push_back(buttonShortRest);

	auto buttonLongRest = vnew Button();
	buttonLongRest->SetText("Long Rest");
	buttonLongRest->SetColor(glm::vec3(0.6f));
	buttonLongRest->SetTextColor(glm::vec3(0.2f));
	buttonLongRest->SetId(ButtonIdLongRest);
	buttonLongRest->SetSize(125.0f, 22.0f);
	buttonLongRest->SetTextSize(18.f);
	children.push_back(buttonLongRest);
}

CardSelection::~CardSelection()
{
}

void CardSelection::Resize(const glm::vec2& windowSize, const TextService& text)
{
	UIView::Resize(windowSize, text);

	position.x = (windowSize.x - size.x);
	position.y = (windowSize.y - size.y) / 2.0f;

	auto x = size.x * 0.5f - 125.f * 0.5f;
	auto y = size.y - (8 + 22.0f) * 3.0f;

	auto button = dynamic_cast<Button*>(children[0]);
	button->SetPosition(x,y);

	y += 30.0f;
	children[1]->SetPosition(x, y);
	
	y += 30.0f;
	children[2]->SetPosition(x, y);
}

void CardSelection::Measure(const glm::vec2& dimensions, const TextService& text)
{
	float width = scalar * texture.width * 2 + 3 * 8.0f;
	float mainHeight = scalar * texture.height + 3 * 8.0f + 22.0f;
	float height = mainHeight + 8 + (8 + 22.0f) * 3.0f; //button height + margins

	size.x = width;
	size.y = height;
}

bool CardSelection::HandleInput(const InputService& input)
{
	if (!UIView::HandleInput(input)) {
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
	UIView::Render(Services);

	Services.Text().PrintCenter(8 + 0.5 * (double)scalar * (double)texture.width, 8.0 + 11.0, "Initiative", 22, Colors::Black);

	glPushMatrix();
	glTranslatef(8, 8 + 8 + 22.0f, 0);
	Render::Quad(0, 0, scalar * texture.width, scalar * texture.height, glm::vec3(0.88f));
	if (playerCards[0] != nullptr) {
		playerCards[0]->Scale(scalar);
		playerCards[0]->Render(Services.Text(), texture);
	}

	glTranslatef(8 + texture.width * scalar, 0, 0);
	Render::Quad(0, 0, static_cast<float>(scalar) * static_cast<float>(texture.width), (float)scalar * (float)texture.height, glm::vec3(0.88f));
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

void CardSelection::OnEvent(WindowEvent type, int id)
{
	if( id == ButtonIdReady )
		callback(*this, 0);
	if(id == ButtonIdShortRest)
		callback(*this, 1);
}

std::string nocard = "no-card";
const std::string& CardSelection::Card(int i)
{
	if (i < 0 || i >= 2)
		return nocard;

	return playerCards[i]->Name();
}

bool CardSelection::CardExists(const PlayerCard& card) const
{
	for( int i = 0; i < 2; ++i ){
		if (playerCards[i] != nullptr && playerCards[i]->Name() == card.Name())
			return true;
	}
	return false;
}
