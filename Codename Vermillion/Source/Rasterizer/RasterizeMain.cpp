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
	render.Draw();

	Triangle t{0.5f, -0.5f, -6.0f, -0.5f, -0.5f, -6.0f, 0.0f, 0.5f, -6.0f};
	render.DrawTriangle(t);

	render.EndFrame();
}
