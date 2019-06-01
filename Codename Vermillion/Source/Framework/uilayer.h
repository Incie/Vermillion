#pragma once

#include"glm/glm.hpp"
#include<vector>
#include<string>

class TextService;
class InputService;
class ServiceLocator;
class Texture;

enum class UILayerId;


class UIElement {
public:
	UIElement() : id(-1), active(true), position(glm::vec2()), size(glm::vec2()), state(UIState::ENABLED), margin(glm::vec2(8,8)) {}
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

	virtual void SetPosition(float x, float y);
	virtual void SetSize(float width, float height);
	virtual void SetMargin(float width, float height);

	const glm::vec2& Margin() { return margin; }
	const glm::vec2& Size() { return size; }
	virtual glm::vec2 Measure(const TextService& text) { return size + margin * 2.0f; }

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
	glm::vec2 margin;
	bool active;
};

class Image : public UIElement {
public:
	Image(const glm::vec2& size, const Texture* texture);
	virtual ~Image();

	virtual void Render(ServiceLocator& Services) override;
	void SetBrightness(float brightness);

protected:
	const Texture* texture;
	glm::vec3 color;
};

class UIText : public UIElement {
public:
	UIText(const std::string& text);
	~UIText();

	virtual void Render(ServiceLocator& Services) override;
	virtual glm::vec2 Measure(const TextService& text);

	void FontHeight(float fontHeight);

	void Text(const std::string& text);
	const std::string Text();

protected:
	std::string text;
	float fontHeight;;
};

class Button : public UIElement {
public:
	Button();
	Button(const std::string& text, unsigned int textSize, int id);
	virtual ~Button();

	virtual glm::vec2 Measure(const TextService& text);
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
	virtual void Resize(const glm::vec2& windowSize, const TextService& text);
	virtual void Measure(const glm::vec2& windowSize, const TextService& text) {}
	virtual void Update();
	virtual void StartRender();
	virtual void Render(ServiceLocator& Services);
	virtual void EndRender();
	virtual void SetSize(float width, float height) {
		UIElement::SetSize(width, height);
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

	void AddChild(UIElement* child);

	bool Invalidated() { return invalidated; }
	void Invalidate() { invalidated = true; }
protected:
	void Resize(const TextService& text);

	bool invalidated;
	std::vector<UIElement*> children;
	int anchor;
};