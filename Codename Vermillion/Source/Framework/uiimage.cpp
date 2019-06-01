#include"pch.h"
#include"uilayer.h"
#include"..//render.h"


Image::Image(const glm::vec2& size, const Texture* texture) 
	: texture(texture), color(glm::vec3(1.0f))
{
	SetSize(size.x, size.y);

}

Image::~Image()
{
	texture = nullptr;
}

void Image::Render(ServiceLocator& Services)
{
	Render::Quad(position, size, *texture, color);
}

void Image::SetBrightness(float brightness)
{
	color.r = brightness;
	color.g = brightness;
	color.b = brightness;
}
