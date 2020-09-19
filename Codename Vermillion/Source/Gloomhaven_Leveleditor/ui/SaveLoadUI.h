#pragma once

#include"..//uiview.h"
#include<functional>

class EditorMainMenu : public UIView
{
public:
	EditorMainMenu(std::function<void(int)> callback);
	~EditorMainMenu();

	void Resize(const glm::vec2& windowSize, const TextService& text);
	void Measure(const glm::vec2& windowSize, const TextService& text);

	void OnEvent(WindowEvent type, int id);
private:
	std::function<void(int)> callback;
};