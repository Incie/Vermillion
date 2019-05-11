#pragma once

#include"../uilayer.h"

class StatusBar : public UILayer {
public:
	StatusBar();
	~StatusBar();

	virtual bool HandleInput(const InputService& inputService);
	virtual void Resize(const glm::vec2& windowSize);
	virtual void Render(ServiceLocator& Services);

	void ResetRounds() { roundNumber = 1; }
	void NextRound() { roundNumber++; }

protected:
	int roundNumber;
};