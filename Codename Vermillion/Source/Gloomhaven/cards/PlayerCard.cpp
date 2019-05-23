#include"pch.h"
#include"PlayerCard.h"
#include"GL/glew.h"
#include"../textures.h"
#include"../render.h"
#include"../icons/icons.h"

PlayerCard::PlayerCard(const std::string& name, std::vector<CardAbility> top, std::vector<CardAbility> bottom, int initiative, int level)
	: name(name), topAction(top), bottomAction(bottom), initiative(initiative), level(level)
{
}

PlayerCard::~PlayerCard()
{
}

void PlayerCard::Render(const TextService& text, const Texture& texture) const
{
	float vertices2[] = {
		0,0,
		texture.width, 0,
		texture.width,texture.height,
		0,texture.height
	};

	float textureCoords[] = {
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f
	};

	glBindTexture(GL_TEXTURE_2D, texture.textureId);
	glTranslatef(0, 0, 0);

	glPushMatrix();
	glScalef(scale.x, scale.y, 1.0f);
	glColor3f(126.0f / 255.0f, 162.0f / 255.0f, 245.0f / 255.0f);
	glBegin(GL_QUADS);
	for (int i = 0; i < 4; ++i) {
		glTexCoord2fv(&textureCoords[i * 2]);
		glVertex2fv(&vertices2[i * 2]);
	}
	glEnd();

	text.PrintCenter(180, 20, name, 25, Colorf(1));
	text.PrintCenter(180, 54, fmt::format("{0}", level), 18, Colorf(1));
	text.PrintCenter(180, 275, fmt::format("{0}", initiative), 40, Colorf(1));

	RenderAbility(text, topAction, glm::vec2(180, 120));
	RenderAbility(text, bottomAction, glm::vec2(180, 330));

	glPopMatrix();
}

bool PlayerCard::PointInside(const glm::vec2& point, double scalar, const Texture& texture) const
{
	if (point.x < 0 || point.y < 0)
		return false;

	if (point.x < texture.width * scalar && point.y < texture.height * scalar )
		return true;

	return false;
}

void PlayerCard::RenderAbility(const TextService& text, const std::vector<CardAbility>& ability, const glm::vec2& position) const
{
	glPushMatrix();
	glTranslatef(position.x, position.y, 0.0f);
	for (auto& cardAbility : ability) {
		int fontHeight = cardAbility.subtitle ? 16 : 25;
		auto width = text.Print(0, 0, cardAbility.name, fontHeight, Colorf(1), true, false);

		glPushMatrix();
		glTranslatef(width, 0, 0);

		auto icon = Icons::Get(cardAbility.name);

		if (icon != nullptr) {
			int fh = fontHeight;
			if (cardAbility.subtitle)
				fh = (fh / 2) * 3;

			auto width = Render::Quad(*icon, fh);
			glTranslatef(width * 0.5f, 0, 0);
		}

		if (cardAbility.value != 0)
			text.Print(0, 0, fmt::format("{0}", cardAbility.value), fontHeight, Colorf(1), true, false);
		glPopMatrix();
		text.NewLine(fontHeight);
	}
	glPopMatrix();
}
