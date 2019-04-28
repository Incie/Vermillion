#include"pch.h"
#include "CardRendering.h"
#include<Windows.h>
#include"GL/glew.h"
#include"../windowstate.h"

#include"../Gloomhaven/cards/PlayerDeck.h"
#include"../render.h"

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
		{CardAbility{"Attack", 3}, CardAbility{"Range", 3, true}, CardAbility{"XP", 1, true}}, 29,	5},
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
	}
};

#include"../Gloomhaven/icons/icons.h"

Texture card0;
Texture card1;
glm::vec2 p;
void CardRendering::Initialize()
{
	auto& textures = Services().Textures();
	card0 = textures.LoadTexture("textures/monster_ability_card_back.png");
	card1 = textures.LoadTexture("textures/player_ability_back.png");

	Icons::Load(textures);
}

void CardRendering::Deinitialize()
{
	Icons::Unload();
}

void CardRendering::Update(double delta)
{
	auto& input = Services().Input();
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



	glPushMatrix();
		glEnable(GL_TEXTURE_2D);

		float scale = (float)WindowState::Width() / (float)cards.size();
		scale /= card1.width;

		for( auto& card : cards ){
			card.Scale(scale);
			card.Render(text, card1);
			glTranslatef((card1.width * scale), 0, 0);
		}
	glPopMatrix();

}