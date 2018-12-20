#include "camera2d.h"
#include"GL/glew.h"


Camera2D::Camera2D() 
	: zoomValue(1.0f),
	  depth(50.0f)
{
}

Camera2D::~Camera2D()
{
}

glm::vec2 Camera2D::ScreenToViewCoords(const glm::vec2& screenCoordNormalized)
{
	UpdateViewport();
	UpdateCameraPosition();

	return positionTopLeft + scaledViewport * screenCoordNormalized;
}

void Camera2D::SetDepth(float newDepth)
{
	depth = newDepth;
}

void Camera2D::SetPositionCenter(const glm::vec2& position)
{
	this->positionTopLeft = position - (viewportSize * 0.5f);
}

void Camera2D::SetPositionTopLeft(const glm::vec2& position)
{
	this->positionTopLeft = position;
}

void Camera2D::SetSize(float width, float height)
{
	viewportSize.x = width;
	viewportSize.y = height;
}

void Camera2D::Move(const glm::vec2& move)
{
	UpdateViewport();
	auto cameraMove = (move / viewportSize);
	cameraMove *= scaledViewport;

	center -= cameraMove;
}

void Camera2D::ZoomByFactor(float factor)
{
	zoomValue *= factor;
}

void Camera2D::ResetZoom()
{
	zoomValue = 1.0f;
}

void Camera2D::Push()
{
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	UpdateViewport();
	UpdateCameraPosition();

	glOrtho(positionTopLeft.x, 
			positionBottomRight.x,
			positionBottomRight.y, 
			positionTopLeft.y,
			-depth, 
			depth);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
}

void Camera2D::Pop()
{
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}

void Camera2D::UpdateViewport()
{
	int viewport[4] = { 0 };
	glGetIntegerv(GL_VIEWPORT, viewport);
	viewportSize.x = (float)viewport[2];
	viewportSize.y = (float)viewport[3];

	scaledViewport = viewportSize * zoomValue;
}

void Camera2D::UpdateCameraPosition()
{
	glm::vec2 halfScaledViewport = scaledViewport * 0.5f;
	positionTopLeft = center - halfScaledViewport;
	positionBottomRight = center + halfScaledViewport;
}


