#pragma once

#include"services.h"

class ServiceLocator
{
public:
	ServiceLocator();
	~ServiceLocator();

	const TextureService& Textures();
private:
	const TextureService* textureService;

};