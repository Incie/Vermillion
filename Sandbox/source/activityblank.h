#pragma once


#include"activity.h"


class ActivityBlank : public Activity
{
public:
	ActivityBlank();
	virtual ~ActivityBlank();

	void Initialize() override;
	void Deinitialize() override;
	void Update(float delta) override;
	void Render() override;

private:

};