#pragma once


#include"activity.h"


class SplashScreen : public Activity
{
public:
	SplashScreen();

	void Initialize() override;
	void Deinitialize() override;
	void Update(float delta) override;
	void Render() override;

private:
	float timer;
	float timerEnd;
};