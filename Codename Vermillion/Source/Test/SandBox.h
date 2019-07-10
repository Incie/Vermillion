#pragma once


#include"..//activity.h"

class SandBox : public Activity {
public:

	virtual void Initialize() override;
	virtual void Deinitialize() override;
	virtual void Update(float delta) override;
	virtual void Render() override;
};