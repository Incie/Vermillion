#pragma once


#include"../uilayer.h"
#include<functional>

class EnemyAdvancer : public UILayer {
public:
	EnemyAdvancer(std::function<void()> callback);
	virtual ~EnemyAdvancer();

	virtual bool HandleInput(const InputService& inputService);
	virtual void Resize(const glm::vec2& windowSize);
	virtual void OnEvent(WindowEvent type, int id);

	UILayerId LayerId() { return UILayerId(); }

private:
	std::function<void()> callback;
};