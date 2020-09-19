#include"pch.h"
#include"uiimage.h"
#include"../render.h"


UIImage::UIImage(const glm::vec2& size, const Texture* texture) 
	: texture(texture), color(glm::vec3(1.0f))
{
	SetSize(size.x, size.y);

}

UIImage::~UIImage()
{
	texture = nullptr;
}

void UIImage::Render(ServiceLocator& Services)
{
	Render::Quad(position, size, *texture, color);
}

void UIImage::SetBrightness(float brightness)
{
	color.r = brightness;
	color.g = brightness;
	color.b = brightness;
}
