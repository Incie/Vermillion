#pragma once

#include"uiview.h"

class TextService;

class VerticalWindow : public UIView {
public:
	VerticalWindow(std::function<void(int)> callback);
	virtual ~VerticalWindow();

	void Measure(const glm::vec2& windowSize, const TextService& text) override;
	void Resize(const glm::vec2& windowSize, const TextService& text) override;
	void OnEvent(WindowEvent windowEvent, int id) override;
private:
	std::function<void(int)> callback;
};