#pragma once


#include"../uiview.h"
#include<functional>

class Texture;

class EnemyAdvancer : public UIView {
public:
	EnemyAdvancer(std::function<void()> callback);
	virtual ~EnemyAdvancer();

	virtual bool HandleInput(const InputService& inputService);
	virtual void Resize(const glm::vec2& windowSize, const TextService& text);
	virtual void Measure(const glm::vec2& windowSize, const TextService& text);
	virtual void OnEvent(WindowEvent type, int id);
	virtual void Render(ServiceLocator& Services);

	void SetEnemyActions(const std::vector<std::string>& actions);
	void Advance();
private:
	std::function<void()> callback;
	int currentAction;
	std::vector<std::string> actions;
	Texture* texture;
};