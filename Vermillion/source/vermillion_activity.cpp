#include"pch.h"
#include"vermillion.h"

#define LogTag "ActivityInterface"

ActivityInterface::ActivityInterface()
{
	TRACE(LogTag);
	toBeStarted = "";
}

void ActivityInterface::RegisterActivityFactory(std::function<Activity* (const std::string&)> activityFactory)
{
	TRACE("ActivityFactory");
	this->activityFactory = activityFactory;
}

void ActivityInterface::StartActivity(const std::string& activityId)
{
	TRACE(LogTag);

	//assert toBeStarted == nullptr
	if (toBeStarted != "")
		throw std::string("Activity already being started!");

	toBeStarted = activityId;
}

Activity* ActivityInterface::ActiveActivity()
{
	if (activities.size() == 0) {
		Log::Error(LogTag, "No activities");
		throw std::string("No registered activities!");
	}

	return activities[0];
}

void ActivityInterface::StartActivityNow(Activity* activity, ServiceLocator& serviceLocator)
{
	TRACE(LogTag)
	activities.push_back(activity);
	
	auto runningActivity = activities[0];
	runningActivity->SetServiceLocator(serviceLocator);
	runningActivity->SetActivityInterface(this);
	runningActivity->Initialize();
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
		for( auto activity : activities ) {
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

	for (auto activity : activities) {
		if (activity != nullptr) {
			activity->Deinitialize();
			delete activity;
			activity = nullptr;
		}
	}

	activities.clear();
}
