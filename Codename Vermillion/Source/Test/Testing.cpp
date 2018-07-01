#include "Testing.h"
#include<Windows.h>
#include<gl/GL.h>

Testing::Testing()
{
}

Testing::~Testing()
{
}

void Testing::Update()
{
}

void Testing::Render()
{
	glBegin(GL_TRIANGLES);
	glVertex2d(50, 50);
	glVertex2d(50, 150);
	glVertex2d(150, 50);
	glEnd();
}
