#pragma once

#include"uielement.h"
#include"glm/vec3.hpp"

class Texture;


class UIImage : public UIElement {
public:
	UIImage(const glm::vec2& size, const Texture* texture);
	virtual ~UIImage();

	virtual void Render(ServiceLocator& Services) override;
	void SetBrightness(float brightness);

protected:
	const Texture* texture;
	glm::vec3 color;
};