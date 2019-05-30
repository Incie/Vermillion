#pragma once


#include"..//uilayer.h"
#include<functional>

class ActionSelector : public UILayer 
{
public:
	ActionSelector(std::function<void(int)> callback);
	~ActionSelector();

	bool HandleInput(const InputService& inputService);
	void Resize(const glm::vec2& windowSize);
	void Update();
	void Render(ServiceLocator& Services);
	void Measure(const glm::vec2& windowSize);

	void OnEvent(WindowEvent type, int id);

	UILayerId LayerId() override;

private:
	std::function<void(int)> callback;
};