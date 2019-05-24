#pragma once

#include"glm/glm.hpp"
#include<vector>

class InputService;
class ServiceLocator;

enum class UILayerId;


class UIElement {
public:
	UIElement() : id(-1), active(true), position(glm::vec2()), size(glm::vec2()), state(UIState::ENABLED) {}
	virtual ~UIElement() {}

	bool IsPointInSide(const glm::vec2& point);

	bool Active() { return active; }
	void Activate() { active = true; }
	void Deactivate() { active = false; }

	enum class UIState {
		ENABLED,
		HOVER,
		ACTIVATED, 
		DISABLED
	};

	virtual void Render(ServiceLocator& Services) = 0;

	virtual void SetPosition(float x, float y) { position = glm::vec2(x, y); }
	virtual void SetSize(float width, float height);

	int Id() { return id; }
	void SetId(int id) { this->id = id; }
	UIState State() { return state; }
	virtual void SetState(UIState newState) { state = newState; }

protected:
	glm::vec2 Center() {
		return glm::vec2(
			position.x + size.x * 0.5f,
			position.y + size.y * 0.5f
		);
	}


	int id;
	UIState state;
	glm::vec2 position;
	glm::vec2 size;
	bool active;
};


class Button : public UIElement {
public:
	Button();
	virtual ~Button();

	virtual void Render(ServiceLocator& Services);
	virtual void SetColor(const glm::vec3& background);
	virtual void SetState(UIState newState);

	void SetTextSize(float textSize) { this->textSize = textSize; }
	void SetTextColor(const glm::vec3& textColor);
	void SetText(const std::string & text) { this->text = text; }

protected:
	std::string text;
	float textSize;
	glm::vec3 color;
	glm::vec3 textColor;

	glm::vec3 enabledTextColor;
	glm::vec3 disabledTextColor;
	glm::vec3 backgroundColor;
	glm::vec3 hoverColor;
	glm::vec3 clickColor;
	glm::vec3 disabledColor;
};



class UILayer : public UIElement {
public:
	UILayer();
	virtual ~UILayer();

	virtual bool HandleInput(const InputService& inputService);
	virtual void Resize(const glm::vec2& windowSize);
	virtual void Update();
	virtual void StartRender();
	virtual void Render(ServiceLocator& Services);
	virtual void EndRender();
	virtual void Measure(const glm::vec2& windowSize) {}
	virtual void SetSize(float width, float height) {
		UIElement::SetSize(width, height);
		Resize();
	}

	virtual UILayerId LayerId() = 0;
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

protected:
	void Resize();

	std::vector<UIElement*> children;
	int anchor;
};