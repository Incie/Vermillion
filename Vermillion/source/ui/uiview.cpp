#include"pch.h"
#include"uiview.h"
#include"../services.h"
#include"../windowstate.h"
#include"GL/glew.h"
#include"../render.h"

UIView::UIView()
	: anchor(WindowAnchor::NONE), invalidated(true), backgroundColor(glm::vec3(1))
{
}

UIView::~UIView() {
	children.clear();
}

bool UIView::HandleInput(const InputService& inputService)
{
	const auto& mouseposition = inputService.GetMousePosition();
	return HandleInput(inputService, *this, mouseposition);
}

void UIView::Measure(const glm::vec2& windowSize, const TextService& text)
{
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

void UIView::BackgroundColor(const glm::vec3& color)
{
	this->backgroundColor = color;
}

bool UIView::HandleInput(const InputService& inputService, UIView& view, const glm::vec2& parentLocalPosition)
{
	if(!view.IsPointInSide(parentLocalPosition)) {
		return false;
	}

	const auto windowLocalPosition = parentLocalPosition - view.Position();
	auto viewChildren = view.Children();
	for(auto child : viewChildren ) {

		if(child->State() == UIElement::UIState::DISABLED)
			continue;

		if(child->HasChildren()) {
			auto childView = std::dynamic_pointer_cast<UIView>(child);
			if( childView )
				HandleInput(inputService, *childView, windowLocalPosition);
		}

		//reset to enabled
		child->SetState(UIElement::UIState::ENABLED);

		if(child->IsPointInSide(windowLocalPosition)) {

			child->SetState(UIElement::UIState::HOVER);

			if(inputService.KeyOnce(VKey_LBUTTON)) {
				child->SetState(UIElement::UIState::ACTIVATED);
				OnEvent(UIView::WindowEvent::CLICK, child->Id());
			}
		}
	}

	return true;
}

void UIView::Resize(const TextService& text) {
	const auto& windowSize = WindowState::Size();
	Resize(glm::vec2((float)windowSize.x, (float)windowSize.y), text);
}

void UIView::SetAnchor(int newAnchor)
{
	anchor = newAnchor;
}