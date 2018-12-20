#pragma once

#include"glm/glm.hpp"

class Camera2D 
{
public:
	Camera2D();
	~Camera2D();

	glm::vec2 ScreenToViewCoords(const glm::vec2& screenCoord);

	void SetDepth(float newDepth);

	void SetPositionCenter(const glm::vec2& position);
	void SetPositionTopLeft(const glm::vec2& position);
	void SetSize(float width, float height);

	void Move(const glm::vec2& move);

	void ZoomByFactor(float factor);
	void ResetZoom();

	void Push();
	void Pop();

	float GetZoomValue() { return zoomValue; }

private:
	void UpdateViewport();
	void UpdateCameraPosition();

	float zoomValue;

	glm::vec2 viewportSize;
	glm::vec2 scaledViewport;

	glm::vec2 positionTopLeft;
	glm::vec2 positionBottomRight;
	glm::vec2 center;
	float depth;
};