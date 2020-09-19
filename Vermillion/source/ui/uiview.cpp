#include"pch.h"
#include"uiview.h"
#include"../services.h"
#include"../windowstate.h"
#include"GL/glew.h"
#include"../render.h"

UIView::UIView()
	: anchor(0), invalidated(true), backgroundColor(glm::vec3(1))
{
}

UIView::~UIView() {
	for(auto child : children)
		delete child;
	children.clear();
}

bool UIView::HandleInput(const InputService& inputService)
{
	const auto& mouseposition = inputService.GetMousePosition();

	if(!IsPointInSide(mouseposition)) {
		return false;
	}

	const auto windowLocalPosition = mouseposition - position;
	for(auto child : children) {

		if(child->State() == UIElement::UIState::DISABLED)
			continue;

		child->SetState(UIElement::UIState::ENABLED);

		if(child->IsPointInSide(windowLocalPosition)) {

			//if( has children )
			 // child->HandleInput( use_this_OnEvent() );

			child->SetState(UIElement::UIState::HOVER);

			if(inputService.KeyOnce(VKey_LBUTTON)) {
				child->SetState(UIElement::UIState::ACTIVATED);
				OnEvent(UIView::WindowEvent::CLICK, child->Id());
			}
		}
	}

	return true;
}

void UIView::Resize(const glm::vec2& windowSize, const TextService& text)
{
	Measure(windowSize, text);

	if(anchor & WindowAnchor::LEFT && anchor & WindowAnchor::RIGHT)
		position.x = windowSize.x * 0.5f - size.x * 0.5f;
	else if(anchor & WindowAnchor::LEFT)
		position.x = 0.0f;
	else if(anchor & WindowAnchor::RIGHT)
		position.x = windowSize.x - size.x;

	if(anchor & WindowAnchor::TOP && anchor & WindowAnchor::BOTTOM)
		position.y = windowSize.y * 0.5f - size.y * 0.5f;
	else if(anchor & WindowAnchor::TOP)
		position.y = 0.0f;
	else if(anchor & WindowAnchor::BOTTOM)
		position.y = windowSize.y - size.y;

	invalidated = false;
}

void UIView::Update(float deltaTime, ServiceLocator& services)
{
	if(active == false)
		return;
}

void UIView::StartRender()
{
	if(active == false)
		return;

	GLCHECK(glPushMatrix());
	GLCHECK(glTranslatef(position.x, position.y, 0.0f));
}

void UIView::Render(ServiceLocator& Services)
{
	if(!active)
		return;

	Render::Quad(0, 0, size.x, size.y, backgroundColor);

	for(auto child : children)
		child->Render(Services);
}

void UIView::EndRender()
{
	if(active == false)
		return;

	GLCHECK(glPopMatrix());
}

void UIView::OnEvent(WindowEvent type, int id)
{
}

void UIView::AddChild(UIElement* child)
{
	children.push_back(child);
}

void UIView::BackgroundColor(const glm::vec3& color)
{
	this->backgroundColor = color;
}

void UIView::Resize(const TextService& text) {
	const auto& windowSize = WindowState::Size();
	Resize(glm::vec2((float)windowSize.x, (float)windowSize.y), text);
}

void UIView::SetAnchor(int newAnchor)
{
	anchor = newAnchor;
}