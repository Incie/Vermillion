#include "Testing.h"
#include"../Framework/input.h"
#include<Windows.h>
#include<gl/GL.h>
#include"glm\glm.hpp"

#include"../Framework/textures.h"



Testing::Testing()
{
	rotation = 0.0;
	position = glm::vec2(250, 250);
	color = glm::vec3(1, 1, 1);
}

Testing::~Testing()
{
}

bool isInitialized = false;
Texture t;

void Testing::Update(double deltaTime)
{
	
	if (!isInitialized) {
		TextureManager texMan;
		t = texMan.LoadTexture("textures/Firelink Shrine sketch.png");
		isInitialized = true;
	}

	PlayerInput *gp = new PlayerInputKeys();
	PlayerInput& input = *gp;
	aimposition = position + input.GetAim(position) * (float)input.GetAimMultiplier();
	auto movement = input.GetMovement();

	position += movement * 100.0f * (float)deltaTime;
	
	if (input.Shoot())
		color = glm::vec3(1, 0, 1);
	else color = glm::vec3(1, 1, 1);

	delete gp;
}

void Testing::Render()
{
	glPushMatrix();
	glTranslated(position.x, position.y, 0);
		glRotated(rotation, 0, 0, 1);
		glColor3f(color.x, color.y, color.z);
		glBegin(GL_TRIANGLES);
			glVertex2d(-25, -25);
			glVertex2d(25, -25);
			glVertex2d(0, 25);
		glEnd();
	glPopMatrix();

	glBegin(GL_LINES);
		glVertex2f(aimposition.x+10, aimposition.y+ 10);
		glVertex2f(aimposition.x- 10, aimposition.y-10);

		glVertex2f(aimposition.x + 10, aimposition.y - 10);
		glVertex2f(aimposition.x - 10, aimposition.y + 10);
	glEnd();


	glPushMatrix();
	glTranslated(250, 250, 0);
	glColor3f(1, 1, 1);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, t.textureId);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glBegin(GL_QUADS);
		double s = 250;
		glTexCoord2d(0, 0); glVertex2d(-s, -s);
		glTexCoord2d(1, 0); glVertex2d( s, -s);
		glTexCoord2d(1, 1); glVertex2d( s,  s);
		glTexCoord2d(0, 1); glVertex2d(-s,  s);
	glEnd();
	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}