#include"pch.h"
#include"uiview_vertical.h"


VerticalWindow::VerticalWindow(std::function<void(int)> callback) 
	: callback(callback)
{
}

VerticalWindow::~VerticalWindow() {
}

void VerticalWindow::Measure(const glm::vec2& windowSize, const TextService& text) {
	glm::vec2 size(0, 0);
	for(auto view : children) {
		view->Measure(text);
		size.x = glm::max(view->Size().x + view->Margin().x * 2.0f, size.x);
		size.y += view->Size().y + view->Margin().y * 2.0f;
	}

	this->size = size;
}

void VerticalWindow::Resize(const glm::vec2& windowSize, const TextService& text) {
	UIView::Resize(windowSize, text);

	size.x = children[0]->Size().x + children[0]->Margin().x * 2.0f;

	glm::vec2 position(0, 0);
	for(auto view : children) {
		position.x = view->Margin().x;
		position.y += view->Margin().y;
		view->SetPosition(position);
		view->SetSize(size.x, view->Size().y);

		position.x = 0.0f;
		position.y += view->Size().y + view->Margin().y;
	}

	size.y = position.y;
}


void VerticalWindow::OnEvent(WindowEvent windowEvent, int id) {
	if(windowEvent == WindowEvent::CLICK) {
		callback(id);
	}
}