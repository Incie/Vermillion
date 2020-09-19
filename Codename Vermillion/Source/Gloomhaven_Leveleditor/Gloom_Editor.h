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

	void Resize(const glm::ivec2& newWindowSize) override;

	void Initialize() override;
	void Deinitialize() override;
	void Update(float delta) override;
	void Render() override;

private:
	std::vector<UIView*> layers;
	EditorBoard editorBoard;
	Camera2D camera;
};