#include "Testing.h"
#include"../Framework/input.h"
#include<Windows.h>
#include<gl/GL.h>

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

	if (InputManager::KeyDown('D'))
		x -= 50 * deltaTime;
	if (InputManager::KeyDown('A'))
		x += 50 * deltaTime;

	if (InputManager::KeyDown('W'))
		y -= 50 * deltaTime;
	if (InputManager::KeyDown('S'))
		y += 50 * deltaTime;
}

void Testing::Render()
{
	glPushMatrix();
	glTranslated(x, y, 0);
		glRotated(rotation, 0, 0, 1);
		glBegin(GL_TRIANGLES);
		glVertex2d(-25, -25);
		glVertex2d(25, -25);
		glVertex2d(0, 25);
		glEnd();
	glPopMatrix();
}
