#pragma once


#include"..//activity.h"
#include"Gloom_EditorBoard.h"
#include"..//camera2d.h"
#include"ui/ActionSelector.h"

class GloomEditor : public Activity 
{
public:
	GloomEditor();
	~GloomEditor();

	void Resize() override;

	void Initialize() override;
	void Deinitialize() override;
	void Update(double delta) override;
	void Render() override;

private:
	std::vector<UILayer*> layers;
	EditorBoard editorBoard;
	Camera2D camera;
};