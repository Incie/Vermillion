#pragma once


#include"activity.h"

#include"editorlevel.h"
#include"camera2d.h"
#include"glm/vec2.hpp"

class ActivityMapEditor : public Activity
{
public:
	ActivityMapEditor();
	virtual ~ActivityMapEditor();

	void Initialize() override;
	void Deinitialize() override;
	void Update(float delta) override;
	void Render() override;

private:
	Camera2D camera;
	EditorLevel level;

	glm::vec2 mousepos;
	glm::vec2 highlight;
};