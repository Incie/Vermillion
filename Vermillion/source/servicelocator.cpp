#include"pch.h"
#include "servicelocator.h"

ServiceLocator::ServiceLocator()
	: textureService(nullptr), inputService(nullptr), eventService(nullptr), soundService(nullptr)
{
}

ServiceLocator::~ServiceLocator()
{
}

InputService& ServiceLocator::Input()
{
	return *inputService;
}

TextService& ServiceLocator::Text()
{
	if(textServices.size() == 0)
		throw std::exception("no textservices registered");
	return *textServices[0];
}

TextService& ServiceLocator::Text(int id)
{
	for(auto ts : textServices) {
		if(ts->ServiceId() == id)
			return *ts;
	}
	
	return *textServices[0];
}

TextureService& ServiceLocator::Textures()
{
	assert(textureService);
	return *textureService;
}

SoundService& ServiceLocator::Sound()
{
	assert(soundService);
	return *soundService;
}

EventService& ServiceLocator::Events()
{
	return *eventService;
}


ServiceAssigner::ServiceAssigner(ServiceLocator& locator)
{
	this->locator = &locator;
}

void ServiceAssigner::SetTextService(TextService& textService)
{
	if(textService.ServiceId() == -1)
		throw std::exception("service id == -1");

	for(auto ts : locator->textServices) {
		if(ts->ServiceId() == textService.ServiceId()) {
			throw std::exception("service id already exists");
		}
	}

	locator->textServices.push_back(&textService);
}

void ServiceAssigner::SetTextureService(TextureService& textureService)
{
	locator->textureService = &textureService;
}

void ServiceAssigner::SetInputService(InputService& inputService)
{
	locator->inputService = &inputService;
}

void ServiceAssigner::SetSoundService(SoundService& soundService)
{
	locator->soundService = &soundService;
}

void ServiceAssigner::SetEventService(EventService& eventService)
{
}
