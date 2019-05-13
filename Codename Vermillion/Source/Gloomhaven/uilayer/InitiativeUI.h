#pragma once

#include"../uilayer.h"

class InitiativeUI : public UILayer {
public:
	InitiativeUI();
	~InitiativeUI();

	virtual bool HandleInput(const InputService& inputService);
	virtual void Resize(const glm::vec2& windowSize);
	virtual void Update();
	virtual void StartRender();
	virtual void Render(ServiceLocator& Services);


	virtual UILayerId LayerId() { return UILayerId(); }
protected:

	//Ordered List of Initiatives
	//Current Initiative Index
};