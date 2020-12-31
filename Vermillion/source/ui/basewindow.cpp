#include"pch.h"
#include"basewindow.h"
#include"../services.h"
#include"../windowstate.h"
#include"../render.h"

BaseWindow::BaseWindow()
{
	id = -1;
	invalidated = true;
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
	return false;
}

void BaseWindow::Render(const TextService& text)
{
	Render::PushMatrix();
		Render::Translate2D(properties.position + glm::vec2{properties.padding.x, properties.padding.y});
		Render::Quad({0,0}, properties.size, properties.backgroundColor);
		text.Print(properties.padding.x, properties.padding.y, properties.title, static_cast<uint32_t>(properties.titleHeight), properties.titleTextColor);

		for(auto& view : views) {
			view->Render(text);
		}
	Render::PopMatrix();
}
