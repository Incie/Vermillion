#include"pch.h"
#include"render.h"

#include"textures.h"
#include"GL/glew.h"


void Render::Quad(const glm::vec2& position, const glm::vec2& size, const Texture& texture, const glm::vec3& color, bool center)
{
	glPushMatrix();
		glTranslatef(position.x, position.y, 0);

		if(center)
			glTranslatef(size.x * -0.5f, size.y * -0.5f, 0.0f);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texture.textureId);

		glColor4f(color.x, color.y, color.z, 1.0f);
		glBegin(GL_QUADS);
			glTexCoord2f(0, 0); glVertex2f(0, 0);
			glTexCoord2f(1, 0); glVertex2f(size.x, 0);
			glTexCoord2f(1, 1); glVertex2f(size.x, size.y);
			glTexCoord2f(0, 1); glVertex2f(0,	   size.y);
		glEnd();
		
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_BLEND);

		glColor4f(1, 1, 1, 1);
	glPopMatrix();
}

float Render::Quad(const glm::vec2& position, Texture& texture, float height)
{
	glPushMatrix();
		glTranslatef(position.x + height * 0.5f, position.y + height * 0.5f, 0.0f);
		Render::Quad(texture, height);
	glPopMatrix();
	return 0.0f;
}

float Render::Quad(const Texture& texture, float height)
{
	const float scale = height / static_cast<float>(texture.height);

	glPushMatrix();
	glScalef(scale, scale, 1);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4f(1, 1, 1, 1);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture.textureId);
	glBegin(GL_QUADS);
	float w = static_cast<float>(texture.width);
	float h = static_cast<float>(texture.height);
	glTexCoord2f(0, 0); glVertex2f(0, 0);
	glTexCoord2f(1, 0); glVertex2f(w, 0);
	glTexCoord2f(1, 1); glVertex2f(w, h);
	glTexCoord2f(0, 1); glVertex2f(0, h);
	glEnd();

	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
	glPopMatrix();

	return texture.width* scale;
}


void Render::Quad(const glm::vec2& position, const glm::vec2& size, const glm::vec3& color) 
{
	Quad(position.x, position.y, size.x, size.y, color);
}

void Render::Quad(float x, float y, float w, float h, const glm::vec3& color)
{
	glPushMatrix();
		glTranslatef(x,y, 0);
		glDisable(GL_TEXTURE_2D);

		glColor3fv(&color.x);
		glBegin(GL_QUADS);
			glVertex2f(0, 0);
			glVertex2f(w, 0);
			glVertex2f(w, h);
			glVertex2f(0, h);
		glEnd();

		glColor3f(1, 1, 1);
		glEnable(GL_TEXTURE_2D);
	glPopMatrix();
}

void Render::OriginCross(float size)
{
	glBegin(GL_LINES);
		glVertex2f(0, -size); glVertex2f(0, size);
		glVertex2f(-size, 0); glVertex2f(size, 0);
	glEnd();
}


//naughty global data
namespace VermillionRenderer {
	float ambientLight[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	float diffuseLight[] = { 0.8f, 0.8f, 0.8f, 1.0f };
	float specularLight[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	glm::vec4 lightDirection = glm::vec4(0.355336f, 0.906561, -0.227779, 0.0);
}

void Render::EnableLight()
{
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);
}

void Render::NoLight()
{
	glDisable(GL_LIGHTING);
	glDisable(GL_LIGHT0);
	glDisable(GL_COLOR_MATERIAL);
}

void Render::SetLightData()
{
	glLightfv(GL_LIGHT0, GL_POSITION, &VermillionRenderer::lightDirection.x);
	glLightfv(GL_LIGHT0, GL_AMBIENT, VermillionRenderer::ambientLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, VermillionRenderer::diffuseLight);
	glLightfv(GL_LIGHT0, GL_SPECULAR, VermillionRenderer::specularLight);
}
