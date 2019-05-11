#include"pch.h"
#include "CardRendering.h"
#include<Windows.h>
#include"GL/glew.h"
#include"../windowstate.h"

#include"../Gloomhaven/icons/icons.h"
#include"../Gloomhaven/cards/PlayerDeck.h"
#include"../render.h"

#include<functional>

#include"../Framework/uilayer.h"

#include"../Gloomhaven/uilayer/CardSelection.h"
#include"../Gloomhaven/uilayer/CardSelector.h"
#include"../Gloomhaven/uilayer/AbilitySelector.h"



Texture card0;
Texture card1;
glm::vec2 p;



class ButtonTest : public UILayer {
public:
	ButtonTest() {
		auto button = new Button();
		button->SetText("Next Turn");
		button->SetId(1);
		button->SetPosition(8, 8);
		button->SetSize(100, 30);
		children.push_back(button);
	}
	virtual ~ButtonTest() {}

	void Resize(const glm::vec2& windowSize)
	{
		UILayer::Resize(windowSize);
	}
	
	void Render(ServiceLocator& Services) {
		UILayer::Render(Services);
	}
	
	void OnEvent(WindowEvent type, int id) {
		if (id == 1) {
			auto button = dynamic_cast<Button*>(children[0]);
			if (button != nullptr)
				button->SetColor(glm::vec3(1, 1, 0));
		}
	}

	virtual UILayerId LayerId() override
	{
		return UILayerId();
	}
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

	auto cardSelector = new CardSelect(cards, card1, [this](const std::string & cardName) {
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

	auto cardSelection = new CardSelection(card1, [this](CardSelection & cs, int eventId) {
		auto cardName0 = cs.Card(0);
		auto cardName1 = cs.Card(1);

		cs.Deactivate();

		auto cardSelector = dynamic_cast<CardSelect*>(layers[0]);
		cardSelector->Deactivate();

		auto abilitySelector = dynamic_cast<AbilitySelector*>(layers[2]);

		auto playerCard0 = std::find_if(cards.begin(), cards.end(), [&cardName0](auto playerCard) { if (playerCard.Name().compare(cardName0) == 0) return true; return false; });
		auto playerCard1 = std::find_if(cards.begin(), cards.end(), [&cardName1](auto playerCard) { if (playerCard.Name().compare(cardName1) == 0) return true; return false; });

		abilitySelector->SetCards(&(*playerCard0), &(*playerCard1));
		abilitySelector->Activate();
	});
	cardSelection->SetSize(0, 0);
	cardSelection->SetAnchor(UILayer::WindowAnchor::RIGHT | UILayer::WindowAnchor::TOP | UILayer::WindowAnchor::BOTTOM);
	cardSelection->Activate();
	layers.push_back(cardSelection);

	//auto buttonTest = new ButtonTest();
	//buttonTest->SetSize(0, 100);
	//buttonTest->SetAnchor(UILayer::WindowAnchor::TOP | UILayer::WindowAnchor::LEFT | UILayer::WindowAnchor::RIGHT);
	//layers.push_back(buttonTest);

	auto abilitySelector = new AbilitySelector(card1);
	abilitySelector->SetCards(&cards[0], &cards[1]);
	abilitySelector->Deactivate();
	layers.push_back(abilitySelector);

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
		if (!layer->Active())
			continue;

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

	cards[0].Scale(1.0f);
	cards[0].Render(text, card1);


	for (auto layer : layers){
		if (!layer->Active())
			continue;

		layer->StartRender();
		layer->Render(Services());
		layer->EndRender();
	}
}