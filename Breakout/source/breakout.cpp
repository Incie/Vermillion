#include"breakout.h" 
#include"render.h"
#include"constants.h"

#include"math.h"

glm::vec2 ballPosition;
glm::vec2 ballDirection;
float ballSpeed;

glm::vec2 paddlePosition;
glm::vec2 paddleSize;


#include"brick.h"
#include"paddle.h"

Brick collisionBrick{glm::vec2{250,600}, glm::vec2{100,25}};

glm::vec2 p0{0,0};
glm::vec2 p1{1000,1000};

glm::vec2 collisionPoint;
glm::vec2 reflectionLine;


bool bAttached = true;
glm::vec2 attachedAt{0, 30.0f};
std::vector<Brick> bricks;
Paddle paddle{glm::vec2{0,0}, glm::vec2{100, 20}};


glm::vec2 windowSize{1,1};
glm::vec2 mousePosition{1,1};

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


	auto& input = Services().Input();
	if( input.KeyDown(VKey_LBUTTON) ) p0 = mousePosition;
	if( input.KeyDown(VKey_RBUTTON) ) p1 = mousePosition;


	paddlePosition.x = mousePosition.x;
	paddle.SetCenterPosition(paddlePosition);

	if( bAttached ){
		ballPosition = paddle.Position() - attachedAt;

		if( input.KeyOnce(VKey_SPACE) ){
			bAttached = false;
			ballDirection = glm::normalize(glm::vec2{0.2f,1.0f});
		}
	} else {

		auto [col, frac, norm] = collisionBrick.Collide(p0, p1-p0, 15);

		if( col ){
			collisionPoint = p0 + (p1-p0) * frac;

			auto reflectedVelocity = (p1-p0) - 2 * glm::dot(norm, (p1-p0)) * norm;
			reflectionLine = reflectedVelocity * 100.0f;
		} else {
			collisionPoint = p1;
			reflectionLine = {0,0};
		}

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

		auto [paddleCollide, paddleFraction, paddleNormal] = paddle.Collide(ballPosition, velocity, 15);

		if( paddleCollide && paddleFraction < fraction )
		{
			collision = true;
			fraction = paddleFraction;
			normal = paddleNormal;
			collisionBrick = nullptr;
		}

		if( collision )
		{
			auto reflectedVelocity = velocity - 2 * glm::dot(normal, velocity) * normal;
			ballDirection = glm::normalize(reflectedVelocity);
			ballPosition = ballPosition + velocity * fraction;

			if( collisionBrick != nullptr )
				collisionBrick->Deactivate();
			else {
				auto paddleCollision = ballPosition - paddle.Position();
				paddleCollision.y = paddle.Position().y + 100 * normal.y;

				auto paddleReflection = glm::normalize(paddleCollision - paddle.Position());
				ballDirection = paddleReflection;

			}


			ballSpeed += 15.0f;
		}
		else 
			ballPosition += ballDirection * ballSpeed * delta;

		if( ballPosition.x > windowSize.x || ballPosition.x < 0.0f )
			ballDirection.x *= -1;
		if( ballPosition.y > windowSize.y || ballPosition.y < 0.0f )
			ballDirection.y *= -1;
	}
}

void BreakoutGame::Render()
{
	Render::NoLight();
	Render::WireFrame();

	Render::Circle(ballPosition, 15, Colors::White);
	paddle.Render();
	
	for( const auto& brick : bricks ){
		brick.Render();
	}


	Render::Line(p0, p1, Colors::Red);
	Render::Line(collisionPoint, collisionPoint + reflectionLine, Colors::Green);
	Render::Circle(collisionPoint, 15, Colors::Blue);
	collisionBrick.Render();

	Render::Fill();
}
