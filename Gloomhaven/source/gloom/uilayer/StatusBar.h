#pragma once

#include"uiview.h"
#include<string>

class StatusBar : public UIView {
public:
	StatusBar();
	~StatusBar();

	virtual bool HandleInput(const InputService& inputService);
	virtual void Resize(const glm::vec2& windowSize, const TextService& text);
	virtual void Measure(const glm::vec2& dimensions, const TextService& text);
	virtual void Render(ServiceLocator& Services);

	void SetStatusText(const std::string& newStatus) { statusText = newStatus; }
	void ResetRounds() { roundNumber = 1; }
	void NextRound() { roundNumber++; }

protected:
	std::string statusText;
	int roundNumber;
};