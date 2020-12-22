#pragma once

#include"ui/uiview.h"
#include"ui/uiview_vertical.h"

#include"textures.h"

class UITileWindow : public VerticalWindow {
public:
	UITileWindow();
	virtual ~UITileWindow();

	void Measure(const glm::vec2& windowSize, const TextService& text) override;
	void Resize(const glm::vec2& windowSize, const TextService& text) override;

	void SetTexture(Texture texture);

private:
	Texture texture;
};