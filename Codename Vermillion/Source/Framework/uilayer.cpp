#include"pch.h"
#include "uilayer.h"
#include"../Framework/services.h"
#include"windowstate.h"
#include"GL/glew.h"

#include<Windows.h>

UILayer::UILayer()
	: anchor(0)
{
	Resize();
}

UILayer::~UILayer() {
	for (auto child : children)
		delete child;
	children.clear();
}

bool UILayer::HandleInput(const InputService & inputService)
{
	const auto& mouseposition = inputService.GetMousePosition();

	if (!IsPointInSide(mouseposition)) {
		return false;
	}

	const auto windowLocalPosition = mouseposition - position;
	for (auto child : children) {

		if (child->State() == UIElement::UIState::DISABLED)
			continue;

		child->SetState(UIElement::UIState::ENABLED);

		if (child->IsPointInSide(windowLocalPosition)) {

			child->SetState(UIElement::UIState::HOVER);

			if (inputService.KeyOnce(VK_LBUTTON)) {
				child->SetState(UIElement::UIState::ACTIVATED);
				OnEvent(UILayer::WindowEvent::CLICK, child->Id());
			}
		}
	}

	return true;
}

void UILayer::Resize(const glm::vec2 & windowSize)
{
	Measure(windowSize);

	if (anchor & WindowAnchor::LEFT && anchor & WindowAnchor::RIGHT)
		position.x = windowSize.x * 0.5f - size.x * 0.5f;
	else if (anchor & WindowAnchor::LEFT)
		position.x = 0.0f;
	else if (anchor & WindowAnchor::RIGHT)
		position.x = windowSize.x - size.x;

	if (anchor & WindowAnchor::TOP && anchor & WindowAnchor::BOTTOM)
		position.y = windowSize.y * 0.5f - size.y * 0.5f;
	else if (anchor & WindowAnchor::TOP)
		position.y = 0.0f;
	else if (anchor & WindowAnchor::BOTTOM)
		position.y = windowSize.y - size.y;
}

void UILayer::Update()
{
	if (active == false)
		return;
}

void UILayer::StartRender()
{
	if (active == false)
		return;

	glPushMatrix();
	glTranslatef(position.x, position.y, 0.0f);
}

#include"render.h"
void UILayer::Render(ServiceLocator & Services)
{
	if (!active)
		return;

	Render::Quad(0, 0, size.x, size.y, glm::vec3(1));

	for (auto child : children)
		child->Render(Services);
}

void UILayer::EndRender()
{
	if (active == false)
		return;

	glPopMatrix();
}

void UILayer::OnEvent(WindowEvent type, int id)
{
}

void UILayer::Resize() {
	const auto& windowSize = WindowState::Size();
	Resize(glm::vec2((float)windowSize.x, (float)windowSize.y));
}



void UILayer::SetAnchor(int newAnchor)
{
	anchor = newAnchor;
}