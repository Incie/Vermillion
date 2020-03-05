#pragma once


#include"ui/uiview.h"

class PortraitPanel : public UIView {
public:
	PortraitPanel();
	virtual ~PortraitPanel();

	virtual bool HandleInput(const InputService& inputService);
	virtual void Resize(const glm::vec2& windowSize, const TextService& text);
	virtual void Render(ServiceLocator& Services);
	
protected:
	//PlayerEntity[]

};