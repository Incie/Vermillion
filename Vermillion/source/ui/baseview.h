#pragma once

#include<vector>
#include<memory>
#include"glm/glm.hpp"
#include"../log.h"
#include"../constants.h"
#include"uitheme.h"

class TextService;
class InputService;

struct ViewState {
	ViewState() : activated(false), mouseInside(false), invalidated(true), disabled(false), visible(true) {}

	bool activated;
	bool mouseInside;
	bool invalidated;
	bool disabled;
	bool visible;
};


class View {
public:
	View();
	virtual ~View() {}

	virtual void Update(const glm::vec2& mousepos) {}
	virtual void Render(const TextService& text) = 0;
	virtual bool HasChildren() { return false; }

	//Properties
	int Id() { return id; }
	void Id(int newId) { id = newId; }

	glm::vec4 Color() const;
	glm::vec4 Foreground() const;

	glm::vec4 Padding() const { return padding; }
	glm::vec4 Margin() const { return margin; }
	glm::vec2 Position() const { return position; }
	glm::vec2 Size() const { return size; }

	void Color(const glm::vec4& newColor) { color = newColor; }
	void Padding(const glm::vec4& newPadding) { padding = newPadding; }
	void Margin(const glm::vec4& newMargin) { margin = newMargin; }
	void Position(const glm::vec2& newPosition) { position = newPosition; }
	void Size(const glm::vec2& newSize) { size = newSize; }

	ViewState& ViewStates() { return viewStates; }
protected:
	int id;

	glm::vec2 position;
	glm::vec2 size;
	glm::vec4 padding;
	glm::vec4 margin;
	glm::vec4 color;

	Theme theme;
	ViewState viewStates;
};

class Button : public View {
public:
	Button() { Log::Debug("View", "Button"); theme = UITheme::Button(); }
	virtual ~Button() {}

	virtual void Render(const TextService& text) override;

	//properties
	void Text(const std::string& newText) { text = newText; }

private:
	std::string text;
};

class Slider : public View {
public:
	Slider();

	virtual void Update(const glm::vec2& mousepos) override;
	virtual void Render(const TextService& text) override;

	double Value();
	void Value(double val);

private:
	double value;
};


class ViewGroup : public View {
public:
	ViewGroup() { Log::Debug("View", "ViewGroup"); }
	virtual ~ViewGroup() = default;

	template<class T>
	T* AddView(int id) {
		children.push_back(std::make_unique<T>());
		auto& v = children.back();
		return dynamic_cast<T*>(v.get());
	}
	
	virtual void Resize(const glm::vec2& windowSize) = 0;
	virtual void Render(const TextService& text) override;

	std::vector< std::unique_ptr<View> >& Children() { return children; }
	virtual bool HasChildren() override { return true; }
protected:
	std::vector< std::unique_ptr<View> > children;
};


class LayoutVertical : public ViewGroup {
public:
	LayoutVertical() { Log::Debug("View", "LayoutVertical"); }
	virtual ~LayoutVertical() = default;

	void Resize(const glm::vec2& windowSize);

private:
};

class LayoutHorizontal : public ViewGroup {
public:
	LayoutHorizontal() { Log::Debug("View", "LayoutHorizontal"); }
	virtual ~LayoutHorizontal() = default;

	void Resize(const glm::vec2& windowSize);

private:
};