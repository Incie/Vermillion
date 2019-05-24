#pragma once

#include"../uilayer.h"
#include<string>

class StatusBar : public UILayer {
public:
	StatusBar();
	~StatusBar();

	virtual bool HandleInput(const InputService& inputService);
	virtual void Resize(const glm::vec2& windowSize);
	virtual void Measure(const glm::vec2& dimensions);
	virtual void Render(ServiceLocator& Services);
	virtual UILayerId LayerId() override;

	void SetStatusText(const std::string& newStatus) { statusText = newStatus; }
	void ResetRounds() { roundNumber = 1; }
	void NextRound() { roundNumber++; }

protected:
	std::string statusText;
	int roundNumber;
};