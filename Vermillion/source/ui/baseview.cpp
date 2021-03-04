#include"pch.h"
#include"baseview.h"
#include"../services.h"

#include"../render.h"

void LayoutVertical::Resize(const glm::vec2& windowSize)
{
	float clientWidth = windowSize.x - padding.x - padding.z;
	float y = margin.y + padding.y;

	Size(windowSize);
	Color({1,0,0,1});

	for(auto& view : children) {
		view->Position({padding.x, y});
		view->Size({clientWidth, view->Size().y});

		y += view->Margin().y + view->Margin().w + view->Size().y;

		if(view->HasChildren()) {
			auto viewGroup = dynamic_cast<ViewGroup*>(view.get());
			viewGroup->Resize(view->Size());
		}
	}
}

void LayoutHorizontal::Resize(const glm::vec2& windowSize)
{
	float clientWidth = windowSize.x - padding.x - padding.z;
	float clientHeight = windowSize.y - padding.y - padding.w;

	Size(windowSize);
	Color({0,1,0,1});

	auto numViews = static_cast<int>(children.size());
	auto viewWidths = clientWidth / static_cast<float>(numViews);

	float x = Padding().x;
	for(auto& view : children) {
		view->Position({x + view->Margin().x, Padding().y});
		view->Size({viewWidths - view->Margin().x - view->Margin().z, clientHeight});

		if(view->HasChildren()) {
			auto viewGroup = dynamic_cast<ViewGroup*>(view.get());
			viewGroup->Resize(view->Size());
		}

		x += viewWidths;
	}
}

void ViewGroup::Render(const TextService& text)
{
	Render::PushMatrix();
		Render::Translate2D(position);
		Render::Quad({0,0}, Size(), Color());
		for(auto& view : children) {
			view->Render(text);
		}
	Render::PopMatrix();
}

void Button::Render(const TextService& text)
{
	Render::Quad(position, size, Color());

	float x = position.x + padding.x;
	float y = position.y + padding.y;
	text.Print(x, y, this->text, 14, Foreground());
}

View::View()
	: padding({4,4,4,4}), margin({2,2,2,2}), color({0.1f, 0.2f, 0.3f, 1.0f}), theme(UITheme::Default())
{
	Log::Debug("View", "View");
}

glm::vec4 View::Color() const
{
	if(viewStates.disabled)
		return theme.bg_disabled;
	if(viewStates.activated)
		return theme.bg_activated;
	if(viewStates.mouseInside)
		return theme.bg_hover;

	return theme.bg;
}

glm::vec4 View::Foreground() const
{
	if(viewStates.disabled)
		return theme.fg_disabled;
	if(viewStates.activated)
		return theme.fg_activated;
	if(viewStates.mouseInside)
		return theme.fg_hover;
	return theme.fg;
}

Slider::Slider()
{
	theme = UITheme::Button();
	value = .25;
}

void Slider::Update(const glm::vec2& mousepos)
{
	if (this->viewStates.activated) {
		if (mousepos.x < 0.0f)
			value = 0.0;
		else if (mousepos.x > size.x)
			value = 1.0;
		else {
			value = mousepos.x / size.x;
		}
	}
}

void Slider::Render(const TextService& text)
{
	auto sizeLeft = glm::vec2{ size.x * (float)value, size.y };
	auto sizeRight = glm::vec2{ size.x * (float)(1.0 - value), size.y };

	Render::Quad(position, sizeLeft, Color());
	Render::Quad(position + glm::vec2{ sizeLeft.x, 0.0f }, sizeRight, Colors::Red);

	auto center = position + size * 0.5f;
	text.Print(center.x, center.y, fmt::format("{}", static_cast<int>(value*100.0)), static_cast<uint32_t>(size.y * 0.5f), Colors::White, true);
}

double Slider::Value()
{
	return value;
}

void Slider::Value(double val)
{
	value = val;
}
