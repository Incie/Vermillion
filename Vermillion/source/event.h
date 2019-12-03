#pragma once

#include"services.h"

class EventManager : EventService 
{
public:
	virtual void DispatchEvent(const Event& eventData) override;
	virtual void RegisterEventHandler(int eventId, const std::function<void(Event*)>& func) override;

private:


};