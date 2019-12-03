#pragma once

#include"services.h"

class ServiceLocator
{
public:
	ServiceLocator();
	~ServiceLocator();

	InputService& Input();
	TextService& Text();
	TextureService& Textures();
	SoundService& Sound();
	EventService& Events();
private:
	friend class ServiceAssigner;

	InputService* inputService;
	TextureService* textureService;
	TextService* textService;
	SoundService* soundService;
	EventService* eventService;
};

class ServiceAssigner
{
public:
	ServiceAssigner(ServiceLocator& locator);

	void SetTextService(TextService& textService);
	void SetTextureService(TextureService& textureService);
	void SetInputService(InputService& inputService);
	void SetSoundService(SoundService& soundService);
	void SetEventService(EventService& eventService);

private:
	ServiceLocator* locator;
};

