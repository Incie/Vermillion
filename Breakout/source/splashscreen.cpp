#include"splashscreen.h"
#include"pch/pch.h"
#include"render.h"


class Animation {
public:
	Animation(float duration) : time(0.0f), duration(duration) {}

	float Start(float atTime = 0.0f) { /* if atTime > 0.0 run animation to atTime*/ time = atTime; }
	float Tick(float deltaTime) { time += deltaTime; }
	bool End() { return time >= duration; }

private:
	float time;
	float duration;
};


class Material {
public:
	Material() : color(1) {}
	Material(const glm::vec4& color) : color(color) {}

	
	const glm::vec4& Color() { return color; }
private:
	glm::vec4 color;
	//texture
};


class Location {
public:
	Location() : position(0), scale(1.0f), rotation(0.0f) {}


private:
	glm::vec3 position;
	glm::vec2 scale;
	float rotation;
};


class Geometry {
public:
	Geometry() : size(0) {}
	Geometry(const glm::vec2& size) : size(size) {}
	Geometry(float width, float height) : size(width, height) {}

	const glm::vec2& Size() { return size; }
private:
	glm::vec2 size;
};


class Sprite {
public:
	Sprite() {}


private:
	Geometry geometry;
	Material material;
	Location location;
};



SplashScreen::SplashScreen()
	: timer(0.0f), timerEnd(1.5f)
{
}

void SplashScreen::Initialize()
{
	TRACE("SplashScreen")
}

void SplashScreen::Deinitialize()
{
	TRACE("SplashScreen")
}

void SplashScreen::Update(float delta)
{
}

void SplashScreen::Render()
{
	auto& text = Services().Text();

	auto width = text.CalculateWidth("VERMILLION", 26);

	auto midX = WindowState::Width() * 0.5f - width * 0.5f;
	auto midY = WindowState::Height() * 0.5f - 26.0f * 0.5f;


	text.Print(midX, midY, "VERMILLION", 26, glm::vec3(1, 1, 1));
}
