#pragma once

#include"pch.h"

class Testing
{
public:
	Testing();
	~Testing();

	void Update(double deltaTime);
	void Render();
	
private:
	glm::vec2 position;
	glm::vec2 aimposition;
	glm::vec3 color;
	double rotation;
};


class PlayerInput
{
public:
	virtual glm::vec2 GetMovement() = 0;
	virtual bool Shoot() = 0;
	virtual glm::vec2 GetAim(const glm::vec2& from) = 0;
	virtual double GetAimMultiplier() = 0;
};

class PlayerInputGamepad : public PlayerInput
{
public:
	virtual glm::vec2 GetMovement() override;
	virtual bool Shoot() override;
	virtual glm::vec2 GetAim(const glm::vec2& from) override;
	virtual double GetAimMultiplier() override;
};

class PlayerInputKeys : public PlayerInput
{
public:
	virtual glm::vec2 GetMovement() override;
	virtual bool Shoot() override;
	virtual glm::vec2 GetAim(const glm::vec2& from) override;
	virtual double GetAimMultiplier() override;
};