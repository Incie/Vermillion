#pragma once

#include<glm/vec2.hpp>

class ServiceLocator;
class TextService;

class UIElement {
public:
	UIElement() : id(-1), active(true), position(glm::vec2()), size(glm::vec2()), state(UIState::ENABLED), margin(glm::vec2(0)) {}
	virtual ~UIElement() {}

	bool IsPointInSide(const glm::vec2& point);

	bool Active() { return active; }
	void Activate() { active = true; }
	void Deactivate() { active = false; }

	enum class UIState {
		ENABLED,
		HOVER,
		ACTIVATED,
		DISABLED,
		GONE //not implemented
	};

	virtual void Render(ServiceLocator& Services) = 0;

	glm::vec2 Position() { return position; }
	virtual void SetPosition(const glm::vec2& newPosition);
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

	virtual bool HasChildren() const { return false; }
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