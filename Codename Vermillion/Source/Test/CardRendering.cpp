#include"pch.h"
#include "CardRendering.h"
#include<Windows.h>

CardRendering::CardRendering()
{
}

CardRendering::~CardRendering()
{
}

Texture card0;
Texture card1;
glm::vec2 p;
void CardRendering::Initialize()
{
	card0 = Services().Textures().LoadTexture("textures/monster_ability_card_back.png");
	card1 = Services().Textures().LoadTexture("textures/player_ability_back.png");
}

void CardRendering::Deinitialize()
{
}

void CardRendering::Update(double delta)
{
	auto& input = Services().Input();
	if (input.KeyDown(VK_RBUTTON)) {
		p += input.GetMouseDelta();
	}
}

#include"GL/glew.h"
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


	float vertices2[] = {
		0,0, 
		card1.width, 0,
		card1.width,card1.height,
		0,card1.height
	};
	glPushMatrix();
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, card1.textureId);
		glTranslatef(0,0, 0);
		glScalef(0.75f, 0.75f, 1.0f);
		glColor3f(126.0f/255.0f, 162.0f/255.0f, 245.0f/255.0f);
		glBegin(GL_QUADS);
			for (int i = 0; i < 4; ++i) {
				glTexCoord2fv(&textureCoords[i * 2]);
				glVertex2fv(&vertices2[i * 2]);
			}
		glEnd();

		text.PrintCenter(180, 20, fmt::format("Juggernaut"), 25, Colorf(1));
		text.PrintCenter(180, 54, fmt::format("1"), 16, Colorf(1));

		text.PrintCenter(180, 120, fmt::format("Move 2"), 25, Colorf(1));
		text.PrintCenter(180, 120+27, fmt::format("Attack 2"), 25, Colorf(1));

		text.PrintCenter(180, 275, fmt::format("34"), 40, Colorf(1));

		text.PrintCenter(180, 330, "on the next three sources of damage", 14, Colorf(1));
		text.PrintCenter(180, 345, "suffer no damage", 14, Colorf(1));

	glPopMatrix();

}



class PlayerCard
{
public:
	PlayerCard();
	~PlayerCard();

	void Render();

private:
	glm::vec2 position;
	glm::vec2 scale;

	std::string name;
	std::vector<std::string> topAction;
	std::vector<std::string> bottomAction;
	int initiative;
	int level;
};

PlayerCard::PlayerCard()
{
}

PlayerCard::~PlayerCard()
{
}

void PlayerCard::Render()
{
}
