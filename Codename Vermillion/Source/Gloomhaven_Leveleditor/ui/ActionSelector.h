#pragma once


#include"..//uiview.h"
#include<functional>

class ActionSelector : public UIView 
{
public:
	ActionSelector(std::function<void(int)> callback);
	~ActionSelector();

	bool HandleInput(const InputService& inputService);
	void Resize(const glm::vec2& windowSize, const TextService& text);
	void Measure(const glm::vec2& windowSize, const TextService& text);
	void Update();
	void Render(ServiceLocator& Services);

	void OnEvent(WindowEvent type, int id);

private:
	std::function<void(int)> callback;
};