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
private:
	friend class ServiceAssigner;

	InputService* inputService;
	TextureService* textureService;
	TextService* textService;
	SoundService* soundService;
};

class ServiceAssigner
{
public:
	ServiceAssigner(ServiceLocator& locator);

	void SetTextService(TextService& textService);
	void SetTextureService(TextureService& textureService);
	void SetInputService(InputService& inputService);
	void SetSoundService(SoundService& soundService);

private:
	ServiceLocator* locator;
};

