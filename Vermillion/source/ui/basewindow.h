#pragma once

#include<vector>
#include<string>
#include<memory>
#include"glm/glm.hpp"
#include"baseview.h"

class TextService;
class InputService;

class BaseWindow {
public:
	enum WindowAnchor {
		None = 0,
		Top = 1,
		Bottom = 2,
		Right = 4,
		Left = 8,
		TopLeft = Top | Left,
		TopRight = Top | Right,
		BottomLeft = Bottom | Left,
		BottomRight = Bottom | Right,
		CenterVertical = Top | Bottom,
		CenterHorizontal = Right | Left,
		CenterTop = Top | CenterHorizontal,
		CenterBottom = Bottom | CenterHorizontal,
		CenterLeft = Left | CenterVertical,
		CenterRight = Right | CenterVertical,
		Center = CenterVertical | CenterHorizontal
	};

	BaseWindow();
	virtual ~BaseWindow();

	void Resize(const TextService& text);
	bool Update(const InputService& input);
	void Render(const TextService& text);


#pragma warning (disable : 26812)
	void Anchor(WindowAnchor newAnchor) { properties.anchor = newAnchor; }
#pragma warning (default : 26812)

	void BackgroundColor(const glm::vec4& color) { properties.backgroundColor = color; }
	void TitleColor(const glm::vec4& color) { properties.titleTextColor = color; }
	int Id() { return id; }
	void Id(int newId) { id = newId; }
	void Invalidate() { invalidated = true; }
	void Position(const glm::vec2& position) { properties.position = position; }
	void Size(const glm::vec2& size) { properties.size = size; }
	void Title(const std::string& title) { properties.title = title; }
	void TitleFontHeight(float newHeight) { properties.titleHeight = newHeight; }
protected:

	struct WindowProperties {
		glm::vec2 position;
		glm::vec2 size;

		glm::vec4 backgroundColor;

		// { left, top, right, bottom }
		glm::vec4 margin;

		// { left, top, right, bottom }
		glm::vec4 padding;

		glm::vec4 border;
		glm::vec4 borderColor;

		std::string title;
		float titleHeight;
		glm::vec4 titleBackgroundColor;
		glm::vec4 titleTextColor;

		WindowAnchor anchor;
	};

	int id;
	bool invalidated;
	bool mouseInside;
	WindowProperties properties;
	Theme theme;


	bool Update(ViewGroup& viewGroup, const glm::vec2& mouseLocal, const InputService& input);
	bool PointInside(const glm::vec2& point);

	void HandleView(View* view, const glm::vec2& point, bool mouseInside);
	bool PointInside(const View* view, const glm::vec2& point);

	bool HandleClick(View* view, const glm::vec2& point);
	bool HandleClickUp(View* view);

	template<class T>
	T* AddView() {
		views.push_back(std::make_unique<T>());
		auto& v = views.back();
		return dynamic_cast<T*>(v.get());
	}

	std::vector<std::unique_ptr<View>> views;
};