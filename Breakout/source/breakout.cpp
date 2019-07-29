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

void BreakoutGame::Initialize()
{
	paddlePosition = glm::vec2(100, 100);
	paddleSize = glm::vec2(100, 20);

	ballPosition = {200,200};
	ballDirection = glm::normalize(glm::vec2{50,50});
	ballSpeed = 250.0f;

	plane = Plane(line0, line1);
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


	collisionPointRectangle = {
		std::fmax(collisionShapePosition.x - collisionShapeSize.x*0.5f, std::fmin(collisionShapePosition.x + collisionShapeSize.x*0.5f, mousePosition.x)),
		std::fmax(collisionShapePosition.y - collisionShapeSize.y*0.5f, std::fmin(collisionShapePosition.y + collisionShapeSize.y*0.5f, mousePosition.y))
	};

	auto collisionDelta = mousePosition - collisionPointRectangle;

	collided = (glm::dot(collisionDelta,collisionDelta) < 15 * 15);
		

	collisionPointSphere = collisionPointRectangle + glm::normalize(mousePosition - collisionPointRectangle) * (glm::distance(mousePosition, collisionPointRectangle) - 15.0f);
}

void BreakoutGame::Render()
{
	Render::NoLight();
	Render::WireFrame();

	Render::Quad(ballPosition, glm::vec2{15,15}, Colors::White);
	Render::Quad(paddlePosition, paddleSize, Colors::Red);
	
	auto color = Colors::Green;
	if( collided ) color = Colors::Red;

	Render::Rectangle(collisionShapePosition, collisionShapeSize, color);
	Render::Circle(mousePosition, 15, color);



	if( Services().Input().KeyDown(VKey_SPACE) )
		startPoint = mousePosition;

	PlaneCollision = plane.Collide(startPoint, mousePosition - startPoint, 25.0f);
	sphereCollisionPoint = startPoint + (mousePosition-startPoint) * PlaneCollision.second;



	Render::Line(line0, line1, Colors::Green);
	Render::Line(startPoint, mousePosition, PlaneCollision.first ? Colors::Red : Colors::Blue );

	Render::Circle(sphereCollisionPoint, 25, Colors::White);

	if( Services().Input().KeyDown(VKey_F1) ){
		Render::Line(collisionPointRectangle, collisionPointSphere, Colors::Green);
		Render::Circle(collisionPointRectangle, 5, Colors::White);
		Render::Circle(collisionPointSphere, 5, Colors::White);
	}

	Render::Fill();
}
