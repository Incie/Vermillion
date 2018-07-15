#include "Testing.h"
#include"../Framework/input.h"
#include<Windows.h>
#include<gl/GL.h>
#include"glm\glm.hpp"


Testing::Testing()
{
	rotation = 0.0;
	position = glm::vec2(250, 250);
	color = glm::vec3(1, 1, 1);
}

Testing::~Testing()
{
}

void Testing::Update(double deltaTime)
{
	PlayerInput *gp = new PlayerInputGamepad();
	PlayerInput& input = *gp;
	aimposition = position + input.GetAim(position) * (float)input.GetAimMultiplier();
	auto movement = input.GetMovement();

	position += movement * 100.0f * (float)deltaTime;
	
	if (input.Shoot())
		color = glm::vec3(1, 0, 1);
	else color = glm::vec3(1, 1, 1);
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
}

double PlayerInputKeys::GetAimMultiplier()
{
	return 0.0;
}
