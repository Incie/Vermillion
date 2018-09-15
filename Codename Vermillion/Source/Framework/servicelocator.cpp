#include "servicelocator.h"

ServiceLocator::ServiceLocator()
	: textService(nullptr),
	  textureService(nullptr)
{
}

ServiceLocator::~ServiceLocator()
{
}

const TextService& ServiceLocator::Text()
{
	return *textService;
}

const TextureService& ServiceLocator::Textures()
{
	return *textureService;
}


ServiceAssigner::ServiceAssigner(ServiceLocator& locator)
{

}

void ServiceAssigner::SetTextService(TextService & textService)
{
}

void ServiceAssigner::SetTextureService(TextureService & textureService)
{
}
