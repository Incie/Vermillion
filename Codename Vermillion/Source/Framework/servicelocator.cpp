#include"pch.h"
#include "servicelocator.h"

ServiceLocator::ServiceLocator()
	: textService(nullptr),
	  textureService(nullptr)
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
	return *textService;
}

TextureService& ServiceLocator::Textures()
{
	return *textureService;
}


ServiceAssigner::ServiceAssigner(ServiceLocator& locator)
{
	this->locator = &locator;
}

void ServiceAssigner::SetTextService(TextService& textService)
{
	locator->textService = &textService;
}

void ServiceAssigner::SetTextureService(TextureService& textureService)
{
	locator->textureService = &textureService;
}

void ServiceAssigner::SetInputService(InputService& inputService)
{
	locator->inputService = &inputService;
}
