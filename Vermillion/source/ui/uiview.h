#pragma once

#include"glm/glm.hpp"
#include<vector>
#include<string>
#include"uielement.h"
#include<memory>

class TextService;
class InputService;
class ServiceLocator;
class Texture;


class UIView : public UIElement {
public:
	UIView();
	virtual ~UIView();

	virtual bool HandleInput(const InputService& inputService);
	virtual void Measure(const glm::vec2& windowSize, const TextService& text);
	virtual void Resize(const glm::vec2& windowSize, const TextService& text);
	virtual void Update(float deltaTime, ServiceLocator& services);
	virtual void StartRender();
	virtual void Render(ServiceLocator& Services);
	virtual void EndRender();
	virtual void SetSize(float width, float height) {
		UIElement::SetSize(width, height);
	}

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
		BOTTOM = 8,
		CENTER_VERTICAL = WindowAnchor::TOP | WindowAnchor::BOTTOM,
		CENTER_HORIZONTAL = WindowAnchor::LEFT | WindowAnchor::RIGHT,
	};

	void SetAnchor(int newAnchor);

	template<class T>
	std::shared_ptr<T> AddChild() {
		auto t = std::make_shared<T>();
		auto element = children.emplace_back(std::dynamic_pointer_cast<UIElement>(t));
		return t;
	}

	void BackgroundColor(const glm::vec3& color);
	bool Invalidated() { return invalidated; }
	void Invalidate() { invalidated = true; }

	virtual bool HasChildren() const override { return true; }
	[[nodiscard]] std::vector<std::shared_ptr<UIElement>>& Children() { return children; }
protected:
	bool HandleInput(const InputService& inputService, UIView& view, const glm::vec2& windowLocalPosition);
	void Resize(const TextService& text);

	glm::vec3 backgroundColor;
	bool invalidated;
	int anchor;

	std::vector<std::shared_ptr<UIElement>> children;
};
