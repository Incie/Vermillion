#pragma once

#include"glm\glm.hpp"


class GamepadTest 
{
public:
	GamepadTest();
	~GamepadTest();

	void Update(double deltaTime);
	void Render();

private:
	void DrawCircle(double radius);
	void DrawCross(const glm::vec2& pos, double length);
	void DrawSlider(const glm::vec2& center, double length, double value);

	glm::vec2 leftPosition;
	glm::vec2 rightPosition;

	bool axyb[4];
	bool dpad[4];
	bool leftStickPressed;
	bool startPressed;
	bool selectPressed;
	bool lBumperPressed;
	bool rBumberPressed;
	float lTrigger;
	float rTrigger;	
};