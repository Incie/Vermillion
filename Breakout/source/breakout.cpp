#include"breakout.h" 
#include"render.h"
#include"constants.h"

#include"math.h"

glm::vec2 ballPosition;
glm::vec2 ballDirection;
float ballSpeed;

glm::vec2 paddlePosition;
glm::vec2 paddleSize;


glm::vec2 collisionShapePosition{300,300};
glm::vec2 collisionShapeSize{100,25};

glm::vec2 mousePosition;

bool collided = false;

glm::vec2 collisionPointRectangle;
glm::vec2 collisionPointSphere;

glm::vec2 windowSize;


Plane plane;
glm::vec2 line0 {100,200};
glm::vec2 line1 {700,200};

glm::vec2 startPoint{0,0};
glm::vec2 sphereCollisionPoint;
std::pair<bool, float> PlaneCollision;

#include"brick.h"
std::vector<Brick> bricks;

void BreakoutGame::Initialize()
{
	paddlePosition = glm::vec2(100, 100);
	paddleSize = glm::vec2(100, 20);

	ballPosition = {200,200};
	ballDirection = glm::normalize(glm::vec2{50,50});
	ballSpeed = 250.0f;

	auto startPosition = glm::vec2{100,100};
	auto brickSize = glm::vec2{100, 25};

	for( int y = 0; y < 5; ++y ){
		for( int i = 0; i < 13; ++i ){
			auto position = glm::vec2{static_cast<float>(i) * (brickSize.x + 10), static_cast<float>(y) * (brickSize.y + 5)  };
			bricks.emplace_back(std::move(Brick(startPosition + position + brickSize*0.5f, brickSize)));
		}
	}
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


	auto velocity = ballDirection * ballSpeed * delta;
	
	bool collision = false;
	float fraction = 1.0f;
	glm::vec2 normal{1,0};
	Brick* collisionBrick = nullptr;
	for( auto& brick : bricks ){
		if( !brick.Active() )
			continue;

		auto [c, f, n] = brick.Collide(ballPosition, velocity, 15);

		if( c && f < fraction ){
			collision = true;
			fraction = f;
			normal = n;
			collisionBrick = &brick;
		}
	}

	if( collision ){
		auto reflectedVelocity = velocity - 2 * glm::dot(normal, velocity) * normal;
		ballDirection = glm::normalize(reflectedVelocity);
		ballPosition = ballPosition + velocity * fraction;
		collisionBrick->Deactivate();
	}
	else 
		ballPosition += ballDirection * ballSpeed * delta;

	if( ballPosition.x > windowSize.x || ballPosition.x < 0.0f )
		ballDirection.x *= -1;
	if( ballPosition.y > windowSize.y || ballPosition.y < 0.0f )
		ballDirection.y *= -1;
}

void BreakoutGame::Render()
{
	Render::NoLight();
	Render::WireFrame();

	Render::Circle(ballPosition, 15, Colors::White);
	Render::Quad(paddlePosition, paddleSize, Colors::Red);
	
	for( const auto& brick : bricks ){
		brick.Render();
	}

	if( Services().Input().KeyDown(VKey_F1) ){
		Render::Line(collisionPointRectangle, collisionPointSphere, Colors::Green);
		Render::Circle(collisionPointRectangle, 5, Colors::White);
		Render::Circle(collisionPointSphere, 5, Colors::White);
	}

	Render::Fill();
}
