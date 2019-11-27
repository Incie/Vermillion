#include"pch.h"
#include"activity.h"

Activity::Activity()
	: serviceLocator(nullptr)
{
}

void Activity::Resize(const glm::ivec2& newWindowSize) {
	for (auto view : layers)
		view->Resize(newWindowSize, Services().Text());
}

void Activity::SetServiceLocator(ServiceLocator& locator) {
	serviceLocator = &locator;
}

void Activity::DeinitializeUI() {
	for (auto view : layers) {
		delete view;
	}

	layers.clear();
}

UIView* Activity::GetViewById(int id) {
	for (auto view : layers) {
		if (view->Id() == id)
			return view;
	}

	return nullptr;
}


void Activity::AddView(UIView* view, int id) {
	view->SetId(id);
	layers.push_back(view);
}

bool Activity::UpdateUI(float deltaTime) {
	bool inputHandled = false;
	for (auto layer : layers) {
		layer->Update(deltaTime, Services());

		if (layer->Active() == false)
			continue;

		if (layer->Invalidated()) {
			auto p = WindowState::Size();
			layer->Resize(glm::vec2(p.x, p.y), Services().Text());
		}

		if (!inputHandled)
			inputHandled = layer->HandleInput(Services().Input());
	}

	return inputHandled;
}

void Activity::RenderUI() {
	auto& services = Services();

	for (auto view : layers) {
		if (view->Active() == false)
			continue;
		view->StartRender();
		view->Render(services);
		view->EndRender();
	}
}