#pragma once

#include"services.h"

class ServiceLocator
{
public:
	ServiceLocator();
	~ServiceLocator();

	const TextService& Text();
	const TextureService& Textures();
private:
	friend class ServiceAssigner;

	const TextureService* textureService;
	const TextService* textService;
};

class ServiceAssigner
{
public:
	ServiceAssigner(ServiceLocator& locator);

	void SetTextService(TextService& textService);
	void SetTextureService(TextureService& textureService);

private:
	ServiceLocator* locator;
};

