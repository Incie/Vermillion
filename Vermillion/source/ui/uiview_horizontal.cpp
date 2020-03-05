#include"pch.h"
#include"uiview_horizontal.h"

HorizontalWindow::HorizontalWindow(std::function<void(int)> callback)
	: callback(callback)
{
}

HorizontalWindow::~HorizontalWindow()
{
}

void HorizontalWindow::Measure(const glm::vec2& windowSize, const TextService& text)
{
}

void HorizontalWindow::Resize(const glm::vec2& windowSize, const TextService& text)
{
}

void HorizontalWindow::OnEvent(WindowEvent windowEvent, int id)
{
}
