#pragma once

#include"activity.h"

class ActivityChess : public Activity {
public:
	ActivityChess();

	void Initialize() override;
	void Deinitialize() override;
	void Update(float delta) override;
	void Render() override;
};