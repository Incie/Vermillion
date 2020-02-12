#pragma once


#include"activity.h"


class Sandbox : public Activity
{
public:
	Sandbox();

	void Initialize() override;
	void Deinitialize() override;
	void Update(float delta) override;
	void Render() override;

private:
	float timer;
	float timerEnd;
};