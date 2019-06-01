#pragma once

#include"../uilayer.h"

class TextService;
class InitiativeTracker;

class InitiativeTrackerUI : public UILayer {
public:
	InitiativeTrackerUI(InitiativeTracker&);
	~InitiativeTrackerUI();

	virtual bool HandleInput(const InputService& inputService);
	virtual void Resize(const glm::vec2& windowSize, const TextService& text);
	virtual void Measure(const glm::vec2& dimensions, const TextService& text);
	virtual void Update();
	virtual void Render(ServiceLocator& Services);


	virtual UILayerId LayerId() { return UILayerId(); }
protected:
	InitiativeTracker& initiativeTracker;

	//Ordered List of Initiatives
	//Current Initiative Index
};