#include "GamepadTest.h"
#include"GL\glew.h"

#include"../Framework/input.h"
#include"..\Framework\window.h"

GamepadTest::GamepadTest()
{
}

GamepadTest::~GamepadTest()
{
}

void GamepadTest::Update(double deltaTime)
{
}

void GamepadTest::Render()
{

	glPushMatrix(); 
		glTranslated(100, 100, 0);
		DrawCircle(25);

		auto leftStickPos = glm::vec2(FrameworkPointers::inputManager->gpX, FrameworkPointers::inputManager->gpY);
		DrawCross(leftStickPos * 25.0f, 5);

	glPopMatrix();

	glPushMatrix();
		glTranslated(150, 150, 0);
		DrawCircle(25);
	glPopMatrix();

	glPushMatrix();
		glTranslated(200, 150, 0);
		DrawCircle(25);
	glPopMatrix();

	glPushMatrix();
		glTranslated(250, 100, 0);
		DrawCircle(10);

		glTranslated(25, 25, 0);
		DrawCircle(10);

		glTranslated(25, -25, 0);
		DrawCircle(10);

		glTranslated(-25, -25, 0);
		DrawCircle(10);
	glPopMatrix();

}

const double TWOPI = 3.14156 * 2.0;
void GamepadTest::DrawCircle(double radius)
{
	glBegin(GL_LINE_LOOP);
	for (auto angle = 0.0; angle < TWOPI; angle += TWOPI / 25.0) 
		glVertex2d(cos(angle) * radius, sin(angle) * radius);
	glEnd();
}

void GamepadTest::DrawCross(const glm::vec2 & pos, double length)
{
	glBegin(GL_LINES);
		glVertex2f(pos.x + length, pos.y + length);
		glVertex2f(pos.x - length, pos.y - length);

		glVertex2f(pos.x + length, pos.y - length);
		glVertex2f(pos.x - length, pos.y + length);
	glEnd();
}

void GamepadTest::DrawSlider(const glm::vec2 & center, double length, double value)
{
}
