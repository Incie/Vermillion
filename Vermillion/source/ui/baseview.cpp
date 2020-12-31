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
	text.Print(x, y, this->text, 14, Colors::White);
}
