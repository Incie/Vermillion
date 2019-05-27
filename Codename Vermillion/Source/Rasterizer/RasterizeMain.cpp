#include"pch.h"
#include "RasterizeMain.h"

RasterizeMain::RasterizeMain()
{
}

RasterizeMain::~RasterizeMain()
{
}

void RasterizeMain::Initialize()
{
}

void RasterizeMain::Deinitialize()
{
}

void RasterizeMain::Update(double delta)
{
}

void RasterizeMain::Render()
{
	render.StartFrame();
	//render.Draw();

	Triangle t{0.5f, -0.5f, 3.0f, 
			  -0.5f, -0.5f, 3.0f, 
		       0.0f,  0.5f, 3.0f};
	render.DrawTriangle(t);
/*
	Triangle t2{0,0,-6.0f, 640,0, -6.0f, 320,240,-6.0f};
	render.DrawTriangle(t2);*/

	render.EndFrame();
}
