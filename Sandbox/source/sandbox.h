#pragma once


#include"activity.h"


class Sandbox : public Activity
{
public:
	Sandbox();
	virtual ~Sandbox();

	void Initialize() override;
	void Deinitialize() override;
	void Update(float delta) override;
	void Render() override;

private:
	void ResetValues();

	float timer;
	float timerEnd;

	std::string displayText;
	std::string goalText;

	float elapsed;
	float timePerThing;
};