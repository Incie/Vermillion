#include"pch.h"
#include"vermillion.h"

#define LogTag "ActivityInterface"

ActivityInterface::ActivityInterface()
{
	TRACE(LogTag);
	toBeStarted = "";
}

void ActivityInterface::RegisterActivityFactory(std::function<std::unique_ptr<Activity> (const std::string&)> activityFactory)
{
	TRACE("ActivityFactory");
	this->activityFactory = activityFactory;
}

void ActivityInterface::StartActivity(const std::string& activityId)
{
	TRACE(LogTag);

	//assert toBeStarted == nullptr
	if (!toBeStarted.empty())
		throw std::string("Activity already being started!");

	toBeStarted = activityId;
}

std::unique_ptr<Activity>& ActivityInterface::ActiveActivity()
{
	if (activities.size() == 0) {
		Log::Error(LogTag, "No activities");
		throw std::string("No registered activities!");
	}

	return activities[0];
}

void ActivityInterface::StartActivityNow(std::unique_ptr<Activity>& activity, ServiceLocator& serviceLocator)
{
	TRACE(LogTag)

	auto& runningActivity = activity;
	runningActivity->SetServiceLocator(serviceLocator);
	runningActivity->SetActivityInterface(this);
	runningActivity->Initialize();

	activities.emplace_back(std::move(activity));
}

bool ActivityInterface::HasQueuedActivity()
{
	return(toBeStarted != "");
}

void ActivityInterface::SetupQueuedActivity(ServiceLocator& serviceLocator)
{
	TRACE(LogTag);

	if (activities.size() > 0) {
		Log::Info(LogTag, "Removing other activities");
		for( auto& activity : activities ) {
			activity->Deinitialize();
		}

		activities.clear();
	}

	auto activity = activityFactory(toBeStarted);
	StartActivityNow(activity, serviceLocator);
	toBeStarted = "";
}

void ActivityInterface::DeinitializeActivities()
{
	TRACE(LogTag);

	for (auto& activity : activities) {
		activity->Deinitialize();
	}

	activities.clear();
}
