#pragma once

#include"uiview.h"

class TextService;

class HorizontalWindow : public UIView {
public:
	HorizontalWindow(std::function<void(int)> callback);
	virtual ~HorizontalWindow();

	void Measure(const glm::vec2& windowSize, const TextService& text) override;
	void Resize(const glm::vec2& windowSize, const TextService& text) override;
	void OnEvent(WindowEvent windowEvent, int id) override;
private:
	std::function<void(int)> callback;
};