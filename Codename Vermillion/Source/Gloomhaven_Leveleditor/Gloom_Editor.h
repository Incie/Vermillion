#pragma once


#include"..//activity.h"

class GloomEditor : public Activity 
{
public:
	GloomEditor();
	~GloomEditor();

	void Initialize() override;
	void Deinitialize() override;
	void Update(double delta) override;
	void Render() override;

private:

};