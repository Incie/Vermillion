#include"pch.h"
#include"basewindow.h"
#include"../services.h"
#include"../windowstate.h"
#include"../render.h"

BaseWindow::BaseWindow()
{
	id = -1;
	invalidated = true;
	mouseInside = false;
	properties.padding = glm::vec4{4,4,4,4};
	properties.margin = glm::vec4{4,4,4,4};
	properties.border = glm::vec4{0};
	properties.anchor = WindowAnchor::None;
	properties.backgroundColor = Colors::ColorFromRGB(0,0,255);
}

BaseWindow::~BaseWindow()
{
}

void BaseWindow::Resize(const TextService& text)
{
	auto windowCenter = glm::vec2(WindowState::Center());

	if(properties.anchor == WindowAnchor::Center) {
		properties.position = windowCenter - properties.size * 0.5f;
	}
	else {
		if(properties.anchor & Left && properties.anchor & Right) {
			properties.position.x = windowCenter.x - properties.size.x * 0.5f;
			properties.size.x = static_cast<float>(WindowState::Width()) - properties.margin.x - properties.margin.z;
		}
		else if(properties.anchor & WindowAnchor::Left) {
			properties.position.x = properties.margin.x;
		}
		else if(properties.anchor & WindowAnchor::Right) {
			properties.position.x = static_cast<float>(WindowState::Width()) - properties.size.x - properties.margin.z;
		}

		if(properties.anchor & Top && properties.anchor & Bottom ) {
			properties.position.y = windowCenter.y - properties.size.y * 0.5f;
			properties.size.y = static_cast<float>(WindowState::Height()) - properties.margin.y - properties.margin.w;
		}
		else if(properties.anchor & WindowAnchor::Top) {
			properties.position.y = properties.margin.y;
		}
		else if(properties.anchor & WindowAnchor::Bottom) {
			properties.position.y = static_cast<float>(WindowState::Height()) - properties.size.y - properties.margin.w;
		}
	}


	for(auto& view : views) {
		auto title = glm::vec2{0.0f, properties.titleHeight + 4.0f};
		auto margin = glm::vec2{properties.margin.x + properties.margin.z, properties.margin.y + properties.margin.w};
		auto viewSize = properties.size - margin - title;
		view->Size();
		view->Position(glm::vec2{properties.margin.x, properties.margin.y} + title);

		if(view->HasChildren()) {
			auto viewGroup = dynamic_cast<ViewGroup*>(view.get());
			viewGroup->Resize(viewSize);
		}
	}
}

bool BaseWindow::Update(const InputService& input) 
{
	const auto& mouseGlobal = input.GetMousePosition();
	const auto mouseLocal = mouseGlobal - this->properties.position;

	auto mouseInside = PointInside(mouseLocal);

	if(mouseInside) {
		if(input.KeyOnce(VKey_LBUTTON)) {
			for (auto& view : views) {
				if (HandleClick(view.get(), mouseLocal))
					break;
			}
		} 
		else if(input.KeyOnceUp(VKey_LBUTTON)) {
			for (auto& view : views) {
				HandleClickUp(view.get());
			}
		}
	}

	if(mouseInside && !this->mouseInside) {
		//entered
		//this->properties.backgroundColor = {1,0,0,0};
	}
	else if(!mouseInside && this->mouseInside) {
		//exited
		//this->properties.backgroundColor = {1,1,0,0};
	}

	this->mouseInside = mouseInside;

	bool inputUsed = false;
	for(auto& view : views) {

		HandleView(view.get(), mouseLocal, mouseInside);

		if(view->HasChildren()) {
			auto viewGroup = dynamic_cast<ViewGroup*>(view.get());

			if( viewGroup != nullptr )
				inputUsed = Update(*viewGroup, mouseLocal - view->Position(), input);
		}
	}

	return inputUsed;
}

bool BaseWindow::Update(ViewGroup& viewGroup, const glm::vec2& mouseLocal, const InputService& input) 
{
	bool inputUsed = false;
	for(auto& view : viewGroup.Children() ) {

		HandleView(view.get(), mouseLocal, mouseInside);

		if(view->HasChildren()) {
			auto viewGroup = dynamic_cast<ViewGroup*>(view.get());

			if(viewGroup != nullptr)
				inputUsed = Update(*viewGroup, mouseLocal - view->Position(), input);
		}
	}

	return inputUsed;
}

bool BaseWindow::PointInside(const glm::vec2& point)
{
	if(point.x < 0 || point.y < 0) {
		return false;
	}

	if(point.x > properties.size.x || point.y > properties.size.y) {
		return false;
	}

	return true;
}

void BaseWindow::HandleView(View* view, const glm::vec2& point, bool mousePotentiallyInside)
{
	if(!mousePotentiallyInside) {
		if(view->ViewStates().mouseInside) {
			view->ViewStates().mouseInside = false;
			view->Color({1,0,0,1});
		}

		return;
	}

	auto mouseLocal = point - view->Position();
	auto mouseInsideView = PointInside(view, mouseLocal);

	if(mouseInsideView && !view->ViewStates().mouseInside) {
		view->ViewStates().mouseInside = true;
		view->Color({0,1,0,1});
	}
	else if(!mouseInsideView && view->ViewStates().mouseInside) {
		view->ViewStates().mouseInside = false;
		view->Color({1,0,0,1});
	}

	view->Update(mouseLocal);
}

bool BaseWindow::PointInside(const View* view, const glm::vec2& point)
{
	if(point.x < 0 || point.y < 0) {
		return false;
	}

	if(point.x > view->Size().x || point.y > view->Size().y) {
		return false;
	}

	return true;
}

bool BaseWindow::HandleClick(View* view, const glm::vec2& point)
{
	auto localPoint = point - view->Position();

	if (!PointInside(view, localPoint))
		return false;

	if (view->HasChildren()) {
		auto viewGroup = dynamic_cast<ViewGroup*>(view);

		if (viewGroup) {
			auto& children = viewGroup->Children();
			for (auto& child : children) {
				bool clicked = HandleClick(child.get(), localPoint);

				if (clicked) {
					return true;
				}
			}
		}

		view->ViewStates().activated = true;
		return true;
	}

	view->ViewStates().activated = true;
	return true;
}

bool BaseWindow::HandleClickUp(View* view)
{
	view->ViewStates().activated = false;

	if (view->HasChildren()) {
		auto viewGroup = dynamic_cast<ViewGroup*>(view);

		if (viewGroup) {
			auto& children = viewGroup->Children();
			for (auto& child : children) {
				bool clicked = HandleClickUp(child.get());
			}
		}
	}

	return true;
}

void BaseWindow::Render(const TextService& text)
{
	Render::PushMatrix();
		Render::Translate2D(properties.position + glm::vec2{properties.padding.x, properties.padding.y});
		Render::Quad({0,0}, properties.size, theme.bg);
		text.Print(properties.padding.x, properties.padding.y, properties.title, static_cast<uint32_t>(properties.titleHeight), theme.fg);

		for(auto& view : views) {
			view->Render(text);
		}
	Render::PopMatrix();
}
