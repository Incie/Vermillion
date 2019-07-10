#include"pch.h"
#include "SandBox.h"


#include"glm/glm.hpp"

#include"..//render.h"


glm::vec2 p0, p1, p;
glm::vec2 p0p1Normal;
glm::vec2 pOnLine;

float dot = 0.0f;
float dotNormalToP;

void SandBox::Initialize()
{
	p0 = glm::vec2(100, 100);
	p1 = glm::vec2(600, 500);
}

void SandBox::Deinitialize()
{
}

void SandBox::Update(float delta)
{
	p = Services().Input().GetMousePosition();

	auto AB = glm::normalize(p1 - p0);
	p0p1Normal = glm::vec2(-AB.y, AB.x);
	auto AC = p - p0;

	auto ABLength = 1.0f; // sqrtf(glm::dot(p1 - p0, p1 - p0));
	dot = glm::dot(AB, AC) / ABLength;
	
	pOnLine = p0 + AB * dot;

	p0p1Normal = glm::vec2(-AB.y, AB.x);

	auto pToPOnLine = glm::normalize(p - pOnLine);
	dotNormalToP = glm::dot(pToPOnLine, p0p1Normal);
}

#include"GL/glew.h"

void Cross(const glm::vec2& position) {
	glPushMatrix();
	glTranslatef(position.x, position.y, 0.0f);
	Render::OriginCross(15.0f);
	glPopMatrix();
}

void Line(const glm::vec2& from, const glm::vec2& to) {
	glBegin(GL_LINES);
	glVertex2fv(&from.x);
	glVertex2fv(&to.x);
	glEnd();
}

void SandBox::Render()
{
	Services().Text().Print(0, 15, fmt::format("dot = {0}", dot), 20, Colors::White, false, false);
	Services().Text().Print(0, 35, dotNormalToP > 0.0f ? "OVER" : "UNDER", 20, Colors::White, false, false);



	Line(p0, p1);
	Line(p0, p);

	glColor3f(0.5f, 1.0f, 1.0f);
	Line(p, p0);
	 
	glm::vec2 lineCenter = p0 + (p1 - p0) * 0.5f;
	Line(lineCenter, lineCenter + p0p1Normal * 25.0f);


	glColor3f(0.5f, 0.5f, 0);
	Line(p, pOnLine);

	glColor3f(0.5f, 0.5f, 1.0f);
	Cross(p);
	Cross(p1);
	Cross(p0);
	Cross(pOnLine);
}
