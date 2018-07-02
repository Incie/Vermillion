#include "Testing.h"
#include<Windows.h>
#include<gl/GL.h>

Testing::Testing()
{
	rotation = 0.0;
}

Testing::~Testing()
{
}

void Testing::Update(double deltaTime)
{
	rotation += 90 * deltaTime;
}

void Testing::Render()
{
	glPushMatrix();
	glTranslated(75, 75, 0);
		glRotated(rotation, 0, 0, 1);
		glBegin(GL_TRIANGLES);
		glVertex2d(-25, -25);
		glVertex2d(25, -25);
		glVertex2d(0, 25);
		glEnd();
	glPopMatrix();
}
