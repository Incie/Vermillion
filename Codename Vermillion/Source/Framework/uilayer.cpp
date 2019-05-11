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
	if (anchor & WindowAnchor::LEFT && anchor & WindowAnchor::RIGHT)
		size.x = windowSize.x;
	if (anchor & WindowAnchor::TOP && anchor & WindowAnchor::BOTTOM)
		size.y = windowSize.y;

	if (anchor & WindowAnchor::LEFT)
		position.x = 0.0f;
	if (anchor & WindowAnchor::RIGHT)
		position.x = windowSize.x - size.x;

	if (anchor & WindowAnchor::TOP)
		position.y = 0.0f;
	if (anchor & WindowAnchor::BOTTOM)
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

void UILayer::Render(ServiceLocator & Services)
{
	if (!active)
		return;

	glDisable(GL_TEXTURE_2D);
	glColor3f(1, 1, 1);
	glBegin(GL_QUADS);
		glVertex2f(0, 0);
		glVertex2f(size.x, 0);
		glVertex2f(size.x, size.y);
		glVertex2f(0, size.y);
	glEnd();
	glEnable(GL_TEXTURE_2D);

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