#pragma once

#include"glm/glm.hpp"
#include<vector>
#include<string>
#include"uielement.h"

class TextService;
class InputService;
class ServiceLocator;
class Texture;


class UIView : public UIElement {
public:
	UIView();
	virtual ~UIView();

	virtual bool HandleInput(const InputService& inputService);
	virtual void Resize(const glm::vec2& windowSize, const TextService& text);
	virtual void Measure(const glm::vec2& windowSize, const TextService& text) {}
	virtual void Update(float deltaTime, ServiceLocator& services);
	virtual void StartRender();
	virtual void Render(ServiceLocator& Services);
	virtual void EndRender();
	virtual void SetSize(float width, float height) {
		UIElement::SetSize(width, height);
	}

	void SetAnchor(int newAnchor);

	enum class WindowEvent {
		CLICK, 
		HOVER,
		ACTIVATE,
		DEACTIVATE
	};

	virtual void OnEvent(WindowEvent type, int id);

	enum WindowAnchor {
		NONE = 0,
		LEFT = 1,
		RIGHT = 2,
		TOP = 4,
		BOTTOM = 8
	};

	void AddChild(UIElement* child);

	void BackgroundColor(const glm::vec3& color);
	bool Invalidated() { return invalidated; }
	void Invalidate() { invalidated = true; }
protected:
	void Resize(const TextService& text);

	glm::vec3 backgroundColor;

	bool invalidated;
	std::vector<UIElement*> children;
	int anchor;
};