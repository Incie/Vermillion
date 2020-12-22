#pragma once


#include"uielement.h"
#include"uiview.h"

class UIGrid : public UIView {
public:
	UIGrid();
	virtual ~UIGrid();

	virtual void Measure(const glm::vec2& windowSize, const TextService& text);
	virtual void Resize(const glm::vec2& windowSize, const TextService& text);

	virtual void Render(ServiceLocator& Services);

private:
	int rowCount;
	int columnCount;
};