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
	glColor3d(1, 1, 1);

	glPushMatrix();
	DrawSlider(glm::vec2(100, 25), 45, FrameworkPointers::inputManager->GamePadTriggerState(false));
	glColor3d(1, 1, 1);
	glPopMatrix();

	glPushMatrix();
		glTranslated(100, 60, 0);
		
		glColor3d(1, 1, 1);
		if (FrameworkPointers::inputManager->GamePadButtonDown(GamePadButton::GP_LeftBumber))
			glColor3d(0, 1, 0);
		DrawCircle(5);
		glColor3d(1, 1, 1);
	glPopMatrix();


	glPushMatrix();
	DrawSlider(glm::vec2(250, 25), 45, FrameworkPointers::inputManager->GamePadTriggerState(true));
	glColor3d(1, 1, 1);
	glPopMatrix();

	glPushMatrix();
	glTranslated(250, 60, 0);

	glColor3d(1, 1, 1);
	if (FrameworkPointers::inputManager->GamePadButtonDown(GamePadButton::GP_RightBumper))
		glColor3d(0, 1, 0);
	DrawCircle(5);
	glColor3d(1, 1, 1);
	glPopMatrix();


	glPushMatrix(); 
		glTranslated(100, 100, 0);
		DrawCircle(25);
		glColor3d(1, 1, 1);
		if (FrameworkPointers::inputManager->GamePadButtonDown(GamePadButton::GP_LeftStick))
			glColor3d(0, 0, 1);
		auto leftStickPos = FrameworkPointers::inputManager->GamePadLeftStick();
		DrawCross(leftStickPos * 15.0f, 5);

	glPopMatrix();
	glColor3f(1, 1, 1);

	glPushMatrix();
		glTranslated(150, 150, 0);
		DrawCircle(25);

		glColor3d(1, 1, 1);
		if (FrameworkPointers::inputManager->GamePadButtonDown(GamePadButton::GP_DLeft))
			glColor3d(0, 0, 1);
		glTranslated(-15, 0, 0);
		DrawCircle(10);

		glColor3d(1, 1, 1);
		if (FrameworkPointers::inputManager->GamePadButtonDown(GamePadButton::GP_DDown))
			glColor3d(1, 1, 0);
		glTranslated(15, 15, 0);
		DrawCircle(10);

		glColor3d(1, 1, 1);
		if (FrameworkPointers::inputManager->GamePadButtonDown(GamePadButton::GP_DRight))
			glColor3d(1, 0, 0);
		glTranslated(15, -15, 0);
		DrawCircle(10);

		glColor3d(1, 1, 1);
		if (FrameworkPointers::inputManager->GamePadButtonDown(GamePadButton::GP_DUp))
			glColor3d(0, 1, 0);
		glTranslated(-15, -15, 0);
		DrawCircle(10);
	glPopMatrix();
	glColor3f(1, 1, 1);

	glPushMatrix();
		glTranslated(200, 150, 0);
		DrawCircle(25);


		glColor3d(1, 1, 1);
		if (FrameworkPointers::inputManager->GamePadButtonDown(GamePadButton::GP_RightStick))
			glColor3d(0, 0, 1);
		auto rightStickPos = FrameworkPointers::inputManager->GamePadRightStick();
		DrawCross(rightStickPos * 15.0f, 5);
	glPopMatrix();


	glPushMatrix();
	glColor3d(1, 1, 1);
	if (FrameworkPointers::inputManager->GamePadButtonDown(GamePadButton::GP_Select))
		glColor3d(0, 0, 1);
	glTranslated(150, 100, 0);
	DrawCircle(10);

	glColor3d(1, 1, 1);
	if (FrameworkPointers::inputManager->GamePadButtonDown(GamePadButton::GP_Start))
		glColor3d(0, 1, 0);
	glTranslated(50, 0, 0);
	DrawCircle(10);
	glPopMatrix();
	glColor3f(1, 1, 1);

	glPushMatrix();
		glColor3d(1, 1, 1);
		if (FrameworkPointers::inputManager->GamePadButtonDown(GamePadButton::GP_X))
			glColor3d(0, 0, 1);
		glTranslated(250, 100, 0);
		DrawCircle(10);

		glColor3d(1, 1, 1);
		if (FrameworkPointers::inputManager->GamePadButtonDown(GamePadButton::GP_A))
			glColor3d(0, 1, 0);
		glTranslated(25, 25, 0);
		DrawCircle(10);

		glColor3d(1, 1, 1);
		if (FrameworkPointers::inputManager->GamePadButtonDown(GamePadButton::GP_B))
			glColor3d(1, 0, 0);
		glTranslated(25, -25, 0);
		DrawCircle(10);

		glColor3d(1, 1, 1);
		if (FrameworkPointers::inputManager->GamePadButtonDown(GamePadButton::GP_Y))
			glColor3d(1, 1, 0);
		glTranslated(-25, -25, 0);
		DrawCircle(10);
	glPopMatrix();
	glColor3f(1, 1, 1);

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
	glTranslated(center.x, center.y, 0);
	double halfLength = length / 2.0;
	double quarterLength = length / 4.0;
	glBegin(GL_LINE_LOOP);
		glVertex2f(-quarterLength, -halfLength);
		glVertex2f(+quarterLength, -halfLength);
		glVertex2f(+quarterLength, +halfLength);
		glVertex2f(-quarterLength, +halfLength);
	glEnd();

	glBegin(GL_LINES);
		glVertex2f(0, -halfLength);
		glVertex2f(0, -halfLength + value * length);
	glEnd();

}
