#include"pch.h"
#include"vermillion.h"
#include"activity.h"

Activity::Activity()
	: serviceLocator(nullptr), activityInterface(nullptr), markedAsFinished(false)
{
	TRACE("Activity");
}

void Activity::Resize(const glm::ivec2& newWindowSize) {
	const auto& textService = Services().Text();
	for (auto view : windows)
		view->Resize(textService);
}

void Activity::SetServiceLocator(ServiceLocator& locator) {
	serviceLocator = &locator;
}

void Activity::SetActivityInterface(ActivityInterface* activityInterface)
{
	this->activityInterface = activityInterface;
}

bool Activity::Finished() const
{
	return markedAsFinished;
}

void Activity::DeinitializeUI() {
	windows.clear();
}

void Activity::StartActivity(const std::string& activityId)
{
	if(activityInterface == nullptr)
		throw std::string("activityinterface == null");

	activityInterface->StartActivity(activityId);
}

std::shared_ptr<BaseWindow> Activity::GetViewById(int id) {
	for (auto view : windows) {
		if (view->Id() == id)
			return view;
	}

	return nullptr;
}


void Activity::Finish()
{
	markedAsFinished = true;
}

void Activity::AddView(BaseWindow* view, int id) {
	throw std::string("Activity::AddView(UIView*, int) deprecated");
}

bool Activity::UpdateUI(float deltaTime) {
	bool inputHandled = false;
	auto& input = Services().Input();


	//if mouseclick
	// check if click hits window, and return inputHandled = true

	//click and drag, how to get mousedeltas to the component?
	//keyboard, gamepad input, how?
	//have a window focusable/focused flag?


	for (auto &window : windows) {

		window->Update(input);

		//layer->Update(deltaTime, Services());

		//if (layer->Active() == false)
		//	continue;

		//if (layer->Invalidated()) {
		//	auto p = WindowState::Size();
		//	layer->Resize(glm::vec2(p.x, p.y), Services().Text());
		//}

		//if (!inputHandled)
		//	inputHandled = layer->HandleInput(Services().Input());
	}

	return inputHandled;
}

void Activity::RenderUI() {
	auto& services = Services();

	for (auto& view : windows) {
		//if (view->Active() == false)
		//	continue;
		view->Render(services.Text());
	}
}