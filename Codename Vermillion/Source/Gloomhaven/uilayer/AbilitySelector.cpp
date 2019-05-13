#include"pch.h"
#include"AbilitySelector.h"
#include"../servicelocator.h"
#include"../cards/PlayerDeck.h"

AbilitySelector::AbilitySelector(Texture& texture, std::function<void(int, int)> callback)
	: callback(callback)
{
	Deactivate();
	SetAnchor(UILayer::WindowAnchor::RIGHT | UILayer::WindowAnchor::TOP | UILayer::WindowAnchor::BOTTOM);

	cardScalar = 0.75f;
	this->texture = &texture;
	cards[0] = nullptr;
	cards[1] = nullptr;

	auto button = new Button();
	button->SetColor(glm::vec3(0.6f));
	button->SetTextColor(glm::vec3(0.3f));
	button->SetText("End Turn");
	button->SetId(1);
	children.push_back(button);

	{
		auto defaultTopButton = new Button();
		defaultTopButton->SetText("DT1");
		defaultTopButton->SetTextSize(16 * cardScalar);
		defaultTopButton->SetId(2);
		defaultTopButton->SetPosition(8, 250 * cardScalar);
		defaultTopButton->SetSize(30 * cardScalar, 20 * cardScalar);
		children.push_back(defaultTopButton);

		auto defaultBottomButton = new Button();
		defaultBottomButton->SetText("DB1");
		defaultBottomButton->SetTextSize(16 * cardScalar);
		defaultBottomButton->SetId(3);
		defaultBottomButton->SetPosition(8, 280 * cardScalar);
		defaultBottomButton->SetSize(30 * cardScalar, 20 * cardScalar);
		children.push_back(defaultBottomButton);

		auto topButton = new Button();
		topButton->SetText("T1");
		topButton->SetTextSize(16 * cardScalar);
		topButton->SetId(4);
		topButton->SetPosition(8 + 150 * cardScalar, 70 * cardScalar);
		topButton->SetSize(30 * cardScalar, 20 * cardScalar);
		children.push_back(topButton);

		auto bottomButton = new Button();
		bottomButton->SetText("B1");
		bottomButton->SetTextSize(16 * cardScalar);
		bottomButton->SetId(5);
		bottomButton->SetPosition(8 + 150 * cardScalar, 300 * cardScalar);
		bottomButton->SetSize(30 * cardScalar, 20 * cardScalar);
		children.push_back(bottomButton);
	}

	{
		float tX = 8 + cardScalar * texture.width + 8;
		auto defaultTopButton = new Button();
		defaultTopButton->SetText("DT2");
		defaultTopButton->SetTextSize(16 * cardScalar);
		defaultTopButton->SetId(6);
		defaultTopButton->SetPosition(tX, 250);
		defaultTopButton->SetSize(30, 20);
		children.push_back(defaultTopButton);

		auto defaultBottomButton = new Button();
		defaultBottomButton->SetText("DB2");
		defaultBottomButton->SetTextSize(16 * cardScalar);
		defaultBottomButton->SetId(7);
		defaultBottomButton->SetPosition(tX, 280);
		defaultBottomButton->SetSize(30 * cardScalar, 20 * cardScalar);
		children.push_back(defaultBottomButton);

		auto topButton = new Button();
		topButton->SetText("T2");
		topButton->SetTextSize(16 * cardScalar);
		topButton->SetId(8);
		topButton->SetPosition(tX + 150 * cardScalar, 70);
		topButton->SetSize(30, 20);
		children.push_back(topButton);

		auto bottomButton = new Button();
		bottomButton->SetText("B2");
		bottomButton->SetTextSize(16 * cardScalar);
		bottomButton->SetId(9);
		bottomButton->SetPosition(tX + 150 * cardScalar, 300);
		bottomButton->SetSize(30 * cardScalar, 20 * cardScalar);
		children.push_back(bottomButton);
	}

}

AbilitySelector::~AbilitySelector()
{
}

bool AbilitySelector::HandleInput(const InputService& inputService)
{
	if (!UILayer::HandleInput(inputService)) {
		return false;
	}

	return true;
}

void AbilitySelector::Resize(const glm::vec2& windowSize)
{
	if (texture == nullptr)
		throw "No texture";
	
	float width = cardScalar * texture->width * 2 + 3 * 8.0f;
	float height = cardScalar * texture->height + 2 * 8.0f;

	size.x = width;
	size.y = height + 20 + 8.0f;

	UILayer::Resize(windowSize);

	auto center = Center();
	size.y = height + 20 + 8.0f;

	position.y = windowSize.y * 0.5f - size.y * 0.5f;

	auto button = dynamic_cast<Button*>(children[0]);
	button->SetSize(150.0f, 20.0f);
	button->SetTextSize(16.0f);
	button->SetPosition(size.x * 0.5f - 75.0f, height);
}

#include"GL/glew.h"
void AbilitySelector::Render(ServiceLocator& Services)
{
	glPushMatrix();
		UILayer::Render(Services);
	glPopMatrix();

	glPushMatrix();
		glTranslatef(8, 8, 0);
		cards[0]->Scale(cardScalar);
		cards[0]->Render(Services.Text(), *texture);

		glTranslatef(8 + texture->width * cardScalar, 0, 0);
		cards[1]->Scale(cardScalar);
		cards[1]->Render(Services.Text(), *texture);
	glPopMatrix();

	for (auto child : children) {
		if (!child->Active())
			continue;
		child->Render(Services);
	}
}

void AbilitySelector::OnEvent(WindowEvent event, int id)
{
	switch (id) {
	case 1: // end turn
		callback(0, 0);
		break;
	case 2: 
		callback(1, 1);
		break;
	case 3:
		callback(1, 2);
		break;
	case 4:
		callback(1, 3);
		break;
	case 5:
		callback(1, 4);
		break;
	case 6:
		callback(2, 1);
		break;
	case 7:
		callback(2, 2);
		break;
	case 8:
		callback(2, 3);
		break;
	case 9:
		callback(2, 4);
		break;
	default: break;
	}
}

void AbilitySelector::SetTexture(Texture* texture)
{
	this->texture = texture;
}

void AbilitySelector::SetCards(PlayerCard* card0, PlayerCard* card1)
{
	cards[0] = card0;
	cards[1] = card1;
}

void AbilitySelector::DisableCard(int cardId)
{
}

void AbilitySelector::DisableTop()
{
}

void AbilitySelector::DisableBottom()
{
}
