#pragma once


#include"activity.h"


class MainMenuActivity : public Activity
{
public:
	MainMenuActivity();

	void Initialize() override;
	void Deinitialize() override;
	void Update(float delta) override;
	void Render() override;

private:
};