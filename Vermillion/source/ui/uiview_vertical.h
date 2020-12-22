#pragma once

#include"uiview.h"

class TextService;

class VerticalWindow : public UIView {
public:
	VerticalWindow() {}
	VerticalWindow(std::function<void(int)> callback);
	virtual ~VerticalWindow();

	void Measure(const glm::vec2& windowSize, const TextService& text) override;
	void Resize(const glm::vec2& windowSize, const TextService& text) override;
	void OnEvent(WindowEvent windowEvent, int id) override;

	void SetCallback(std::function<void(int)> callback) {
		this->callback = callback;
	}
private:
	std::function<void(int)> callback;
};