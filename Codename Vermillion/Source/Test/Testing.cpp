#include "Testing.h"
#include"../Framework/input.h"
#include<Windows.h>
#include<gl/GL.h>
#include"glm\glm.hpp"

#include"../Framework/window.h"

glm::vec2 position;

Testing::Testing()
{
	rotation = 0.0;
	x = 75;
	y = 75;
}

Testing::~Testing()
{
}

void Testing::Update(double deltaTime)
{
	rotation += 90 * deltaTime;

	if (FrameworkPointers::inputManager->KeyDown(VK_LBUTTON)) {
		auto p = FrameworkPointers::inputManager->mousePosition;
		auto moveVec = glm::vec2(p.x - position.x, p.y - position.y);
		moveVec = glm::normalize(moveVec);

		position += moveVec * 50.0f * (float)deltaTime;

	}

	if (FrameworkPointers::inputManager->KeyDown('A'))
		position.x -= 50 * deltaTime;
	if (FrameworkPointers::inputManager->KeyDown('D'))
		position.x += 50 * deltaTime;

	if (FrameworkPointers::inputManager->KeyDown('S'))
		position.y -= 50 * deltaTime;
	if (FrameworkPointers::inputManager->KeyDown('W'))
		position.y += 50 * deltaTime;
}

void Testing::Render()
{
	glPushMatrix();
	glTranslated(position.x, position.y, 0);
		glRotated(rotation, 0, 0, 1);
		glBegin(GL_TRIANGLES);
		glVertex2d(-25, -25);
		glVertex2d(25, -25);
		glVertex2d(0, 25);
		glEnd();
	glPopMatrix();
}
