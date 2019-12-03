#include"pch.h"
#include "event.h"

void EventManager::DispatchEvent(const Event& eventData)
{
}

void EventManager::RegisterEventHandler(int eventId, const std::function<void(Event*)>& func)
{
}
