#pragma once


#include"../uilayer.h"

class PortraitPanel : public UILayer {
public:
	PortraitPanel();
	virtual ~PortraitPanel();

	virtual bool HandleInput(const InputService& inputService);
	virtual void Resize(const glm::vec2& windowSize);
	virtual void Render(ServiceLocator& Services);
	
protected:
	//PlayerEntity[]

};