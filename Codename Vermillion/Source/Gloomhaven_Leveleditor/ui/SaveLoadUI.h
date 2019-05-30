#pragma once

#include"..//uilayer.h"
#include<functional>

class EditorMainMenu : public UILayer
{
public:
	EditorMainMenu(std::function<void(int)> callback);
	~EditorMainMenu();

	void Resize(const glm::vec2& windowSize);
	void Measure(const glm::vec2& windowSize);

	void OnEvent(WindowEvent type, int id);

	UILayerId LayerId() override;

private:
	std::function<void(int)> callback;
};