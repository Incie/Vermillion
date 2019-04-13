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
private:
	friend class ServiceAssigner;

	InputService* inputService;
	TextureService* textureService;
	TextService* textService;
};

class ServiceAssigner
{
public:
	ServiceAssigner(ServiceLocator& locator);

	void SetTextService(TextService& textService);
	void SetTextureService(TextureService& textureService);
	void SetInputService(InputService& inputService);

private:
	ServiceLocator* locator;
};

