#pragma once


#include"..//activity.h"

class SandBox : public Activity {
public:

	virtual void Initialize() override;
	virtual void Deinitialize() override;
	virtual void Update(double delta) override;
	virtual void Render() override;
};