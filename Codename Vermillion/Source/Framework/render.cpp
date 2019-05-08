#include"pch.h"
#include"render.h"

#include"textures.h"
#include"GL/glew.h"

float Render::Quad(const Texture& texture, float height)
{
	float scale = height / texture.height;

	glPushMatrix();
	glScalef(scale, scale, 1);
	glTranslatef(-texture.width, 0, 0);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4f(1, 1, 1, 1);

	glBindTexture(GL_TEXTURE_2D, texture.textureId);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0); glVertex2f(0, 0);
	glTexCoord2f(1, 0); glVertex2f(texture.width, 0);
	glTexCoord2f(1, 1); glVertex2f(texture.width, texture.height);
	glTexCoord2f(0, 1); glVertex2f(0, texture.height);
	glEnd();

	glDisable(GL_BLEND);
	glPopMatrix();

	return texture.width* scale;
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
