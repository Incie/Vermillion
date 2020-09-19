#pragma once

#include"activity.h"

class MenuTest : public Activity
{
public:
	MenuTest();

	void Initialize() override;
	void Deinitialize() override;
	void Update(float delta) override;
	void Render() override;

private:
};