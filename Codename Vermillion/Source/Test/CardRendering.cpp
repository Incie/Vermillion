#include"pch.h"
#include "CardRendering.h"
#include<Windows.h>
#include"GL/glew.h"
#include"../windowstate.h"

#include"../Gloomhaven/icons/icons.h"
#include"../Gloomhaven/cards/PlayerDeck.h"
#include"../render.h"

#include<functional>

class UILayer {
public:
	UILayer() { active = false; }
	virtual ~UILayer() {}

	void Activate() { active = true; }
	void Deactivate() { active = false; }

	virtual bool HandleInput(const InputService& inputService) {
		const auto& mpos = inputService.GetMousePosition();

		if (!IsPointInSide(mpos)) {
			return false;
		}

		return true;
	}

	bool IsPointInSide(const glm::vec2& point) {
		if (point.x > position.x && point.x < (position.x + size.x)) {
			if (point.y > position.y && point.y < (position.y + size.y))
				return true;
		}

		return false;
	}

	virtual void Resize(const glm::vec2& windowSize) {
		if (anchor & WindowAnchor::LEFT && anchor & WindowAnchor::RIGHT)
			size.x = windowSize.x;
		if (anchor & WindowAnchor::TOP && anchor & WindowAnchor::BOTTOM)
			size.y = windowSize.y;

		if(anchor & WindowAnchor::LEFT)
			position.x = 0.0f;
		if (anchor & WindowAnchor::RIGHT) 
			position.x = windowSize.x - size.x;

		if (anchor & WindowAnchor::TOP)
			position.y = 0.0f;
		if (anchor & WindowAnchor::BOTTOM)
			position.y = windowSize.y - size.y;
	}

	virtual void Update() {
		if (active == false)
			return;
	}

	virtual void StartRender() {
		if (active == false)
			return;

		glPushMatrix();
		glTranslatef(position.x, position.y, 0.0f);
	}

	virtual void Render(ServiceLocator& Services){
		glDisable(GL_TEXTURE_2D);
		glColor3f(1, 1, 1);
		glBegin(GL_QUADS);
			glVertex2f(0, 0);
			glVertex2f(size.x, 0);
			glVertex2f(size.x, size.y);
			glVertex2f(0, size.y);
		glEnd();
		glEnable(GL_TEXTURE_2D);
	}

	virtual void EndRender() {
		if (active == false)
			return;

		glPopMatrix();
	}


	enum WindowAnchor {
		NONE = 0,
		LEFT = 1,
		RIGHT = 2,
		TOP = 4,
		BOTTOM = 8
	};

	void SetSize(float width, float height) {
		size.x = width;
		size.y = height;
	}

	void SetAnchor(int newAnchor) {
		anchor = newAnchor;
	}

protected:
	int anchor;

	glm::vec2 position;
	glm::vec2 size;

	bool active;
};


Texture card0;
Texture card1;
glm::vec2 p;



class CardSelection : public UILayer {
public:
	CardSelection()
	{
		scalar = 0.66f;
		playerCards[0] = nullptr;
		playerCards[1] = nullptr;
	}

	void Resize(const glm::vec2& windowSize) {
		UILayer::Resize(windowSize);

		float width = scalar * card1.width * 2 + 3 * 8.0f;
		float height = scalar * card1.height + 3 * 8.0f + 22.0f;
		
		size.x = width;
		size.y = height;

		position.y = (windowSize.y - size.y) / 2.0f;
	}

	bool HandleInput(const InputService& input) {
		if (!UILayer::HandleInput(input)) {
			return false;
		}

		glm::vec2 mousePosition = input.GetMousePosition() - position;


		if( input.KeyOnce(VK_LBUTTON) )
		{
			if (playerCards[0] != nullptr) {
				if (playerCards[0]->PointInside(mousePosition - glm::vec2(8, 16 + 22), scalar, card1))
					playerCards[0] = nullptr;
			}

			if (playerCards[1] != nullptr) {
				if (playerCards[1]->PointInside(mousePosition - glm::vec2(8 + 8 + card1.width * scalar, 16 + 22), scalar, card1))
					playerCards[1] = nullptr;
			}
		}

		return true;
	}

	void Render(ServiceLocator& Services) {
		UILayer::Render(Services);

		Services.Text().PrintCenter(8 + 0.5f * scalar * card1.width, 8 + 11.0f, "Initiative", 22.0f, Colorf(0));

		float w = card1.width;
		float h = card1.height;

		glPushMatrix();
			glTranslatef(8, 8+8 + 22.0f, 0);
				Render::Quad(0, 0, scalar * card1.width, scalar * card1.height, glm::vec3(0.88));
				if (playerCards[0] != nullptr) {
					playerCards[0]->Scale(scalar);
					playerCards[0]->Render(Services.Text(), card1);
				}
			
				glTranslatef(8 + card1.width * scalar, 0, 0);
			Render::Quad(0,0, scalar * card1.width, scalar * card1.height, glm::vec3(0.88));
			if (playerCards[1] != nullptr){
				playerCards[1]->Scale(scalar);
				playerCards[1]->Render(Services.Text(), card1);
			}
		glPopMatrix();
	}

	void AddCard(const PlayerCard& playerCard) {
		if (playerCards[0] == nullptr)
			playerCards[0] = &playerCard;
		else if (playerCards[1] == nullptr)
			playerCards[1] = &playerCard;
	}

	bool IsFull() const {

	}

protected:
	PlayerCard const *playerCards[2];
	float scalar;

};


class CardSelect : public UILayer {
public:
	CardSelect(std::vector<PlayerCard>& cards, std::function<void(const std::string&)> onclick) 
	{
		onClick = onclick;

		cardHighlight = -1;
		for (auto& playerCard : cards)
			cardList.push_back(&playerCard);
	}

	virtual ~CardSelect() {
		cardList.clear();
	}

	void Resize(const glm::vec2& windowSize) {
		UILayer::Resize(windowSize);

		scale = size.x / (float)cardList.size();
		scale /= card1.width;

		float maxScale = size.y / card1.height;
		scale = std::min<float>(maxScale, scale);

		totalWidth = cardList.size() * (card1.width * scale);
		centerTranslate = (size.x - totalWidth) / 2.0f;

		position.x = centerTranslate;
		size.x = totalWidth;
	}

	bool HandleInput(const InputService& input) {
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


	void Render(ServiceLocator& Services) {
		UILayer::Render(Services);

		glPushMatrix();
		glTranslatef(0, 0, 0.1f);
		glEnable(GL_TEXTURE_2D);

		if (cardHighlight != -1) {
			glPushMatrix();
			glTranslatef(0, -0.75f * card1.height, 0);
			cardList[cardHighlight]->Scale(0.75f);
			cardList[cardHighlight]->Render(Services.Text(), card1);
			glPopMatrix();
		}

		for (auto card : cardList) {
			card->Scale(scale);
			card->Render(Services.Text(), card1);
			glTranslatef((card1.width * scale), 0, 0);
		}
		glPopMatrix();
	}

protected:

	std::function<void(const std::string&)> onClick;

	int cardHighlight;

	float centerTranslate;
	float scale;
	float totalWidth;

	std::vector<PlayerCard*> cardList;
};


CardRendering::CardRendering()
{
}

CardRendering::~CardRendering()
{
}

std::vector<PlayerCard> cards{
	PlayerCard{
		"Immovable Phalanx",
		{CardAbility{"Attack", 4}, CardAbility{"Shield", 1}, CardAbility{"self", 0, true}},
		{}, 17,	6
	},
	PlayerCard{
		"Skirmishing Manouver",
		{CardAbility{"Attack", 2}, CardAbility{"Move", 2},CardAbility{"Attack", 2}},
		{CardAbility{"Attack", 3}, CardAbility{"Range", 3, true}, CardAbility{"XP", 1, true}}, 29,	5
	},
	PlayerCard{
		"Unstoppable Charge",
		{CardAbility{"Attack", 5}, CardAbility{"XP", 1}},
		{CardAbility{"Move", 4}, CardAbility{"STUN"}, CardAbility{"All adjacent enemies"}, CardAbility{"LOSS"}},
		86, 4
	},
	PlayerCard{
		"Juggernaut",
		{CardAbility{"Move", 2}, CardAbility{"Attack", 2}},
		{},
		34, 2
	},
	PlayerCard{
		"Hook and Chain",
		{CardAbility{"Attack", 3}, CardAbility{"Range", 3, true}, CardAbility{"PULL", 2, true}},
		{CardAbility{"Move", 4}},
		42, 3
	},

	PlayerCard{
		"Leaping Cleave",
		{CardAbility{"Attack", 3}, CardAbility{"Target", 2, true}, CardAbility{"XP", 1}},
		{CardAbility{"Move", 3}, CardAbility{"Jump"}},
		54, 1
	},
	PlayerCard{
		"Trample",
		{CardAbility{"Attack", 3}, CardAbility{"Pierce", 2, true}},
		{CardAbility{"Move", 4}, CardAbility{"Jump", 0, true}, CardAbility{"Attack", 2}, CardAbility{"Target all enemies moved through",0, true}, CardAbility{"XP", 2}, CardAbility{"LOSS"}},
		72, 1
	},
	PlayerCard{
		"Skewer",
		{CardAbility{"Attack", 3}, CardAbility{"Range", 3, true}, CardAbility{"PULL", 2, true}},
		{CardAbility{"Move", 4}},
		35, 1
	},
	PlayerCard{
		"Spare Dagger",
		{CardAbility{"Attack", 3}, CardAbility{"Range", 3, true}, CardAbility{"XP", 1}},
		{CardAbility{"Attack", 2}},
		27, 1
	},
	PlayerCard{
		"Provoking Roar",
		{CardAbility{"Attack", 2}, CardAbility{"DISARM", 0, true}},
		{CardAbility{"Move", 2}},
		10, 1
	},
};



void CardRendering::Initialize()
{
	auto& textures = Services().Textures();
	card0 = textures.LoadTexture("textures/monster_ability_card_back.png");
	card1 = textures.LoadTexture("textures/player_ability_back.png");

	std::sort(cards.begin(), cards.end(), [](auto a, auto b) { return a.Initiative() < b.Initiative(); });

	Icons::Load(textures);

	auto cardSelector = new CardSelect(cards, [this](const std::string & cardName) {
		auto cardFound = std::find_if(cards.begin(), cards.end(), [&cardName](auto playerCard) { if (playerCard.Name().compare(cardName) == 0) return true; return false; });
		if (cardFound == cards.end())
			throw "card not found";

		auto cardLayer = dynamic_cast<CardSelection*>(layers[1]);
		if (cardLayer == nullptr)
			throw "layer not found";

		cardLayer->AddCard(*cardFound);
	} );
	cardSelector->SetSize(0, 150.0f);
	cardSelector->SetAnchor(UILayer::WindowAnchor::BOTTOM | UILayer::WindowAnchor::LEFT | UILayer::WindowAnchor::RIGHT);
	cardSelector->Activate();
	layers.push_back(cardSelector);


	auto cardSelection = new CardSelection();
	cardSelection->SetSize(0, 0);
	cardSelection->SetAnchor(UILayer::WindowAnchor::RIGHT | UILayer::WindowAnchor::TOP | UILayer::WindowAnchor::BOTTOM);
	cardSelection->Activate();
	layers.push_back(cardSelection);
}

void CardRendering::Deinitialize()
{
	for (auto layer : layers)
		delete layer;
	layers.clear();

	Icons::Unload();
}

#include"../Framework/log.h"
void CardRendering::Resize()
{
	Log::Info("CardRendering", "Resize");
	const auto& windowSize = WindowState::Size();
	glm::vec2 newWindowSize{ windowSize.x, windowSize.y };
	for (auto layer : layers)
		layer->Resize(newWindowSize);
}

void CardRendering::Update(double delta)
{
	auto& input = Services().Input();

	bool inputHandled = false;
	for (auto layer : layers) {
		if( !inputHandled )
			inputHandled = layer->HandleInput(input);
		layer->Update();
	}


	if (input.KeyDown(VK_RBUTTON)) {
		p += input.GetMouseDelta();
	}
}

void CardRendering::Render()
{
	auto m = Services().Input().GetMousePosition();
	auto& text = Services().Text();
	text.PrintCenter(500, 20, fmt::format("{0}, {1}", m.x, m.y), 30, Colorf(1));

	text.PrintCenter(500, 40, fmt::format("{0}, {1}", p.x, p.y), 30, Colorf(1));

	float textureCoords[] = {
		0.0f, 0.0f, 
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f
	};

	float vertices[] = {
		0.0f, 0.0f, 
		400.0f, 0.0f,
		400.0f, 200.0f,
		0.0f, 200.0f
	};

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, card0.textureId);

	glPushMatrix();
		glTranslatef(p.x, p.y, 0);
		glBegin(GL_QUADS);
		for (int i = 0; i < 4; ++i) {
			glTexCoord2f(textureCoords[i * 2], textureCoords[i * 2 + 1]);
			glVertex2f(vertices[i * 2], vertices[i * 2 + 1]);
		}
		glEnd();

		text.PrintCenter(37, 50, fmt::format("{0}", 8), 25, Colorf(1));

		glPushMatrix();
			text.PrintCenter(210, 70, fmt::format("Move {0}", 2), 25, Colorf(1));
			text.PrintCenter(210, 70+25, fmt::format("Attack {0}", 2), 25, Colorf(1));
		glPopMatrix();
	glPopMatrix();


	for (auto layer : layers){
		layer->StartRender();
		layer->Render(Services());
		layer->EndRender();
	}
}