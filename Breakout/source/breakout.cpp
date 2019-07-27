#include"breakout.h" 
#include"render.h"
#include"constants.h"

glm::vec2 ballPosition;
glm::vec2 ballDirection;
float ballSpeed;

glm::vec2 paddlePosition;
glm::vec2 paddleSize;


glm::vec2 collisionShapePosition{300,300};
glm::vec2 collisionShapeSize{100,25};

glm::vec2 mousePosition;


glm::vec2 windowSize;

void BreakoutGame::Initialize()
{
	paddlePosition = glm::vec2(100, 100);
	paddleSize = glm::vec2(100, 20);

	ballPosition = {200,200};
	ballDirection = glm::normalize(glm::vec2{50,50});
	ballSpeed = 250.0f;
}

void BreakoutGame::Deinitialize()
{
}

void BreakoutGame::Resize(const glm::ivec2& newWindowSize)
{
	windowSize = newWindowSize;
	Activity::Resize(newWindowSize);

	paddlePosition.y = static_cast<float>(newWindowSize.y) - 100;
}

void BreakoutGame::Update(float delta)
{
	auto mousePosition = Services().Input().GetMousePosition();
	::mousePosition = mousePosition;

	paddlePosition.x = mousePosition.x - paddleSize.x * 0.5f;

	ballPosition += ballDirection * ballSpeed * delta;
	
	if( ballPosition.x > windowSize.x || ballPosition.x < 0.0f )
		ballDirection.x *= -1;
	if( ballPosition.y > windowSize.y || ballPosition.y < 0.0f )
		ballDirection.y *= -1;
}

void BreakoutGame::Render()
{
	Render::Quad(ballPosition, glm::vec2{15,15}, Colors::White);
	Render::Quad(paddlePosition, paddleSize, Colors::Red);

	Render::Quad(collisionShapePosition, collisionShapeSize, Colors::Green);
	Render::Quad(mousePosition, {15,15}, Colors::White);
}
