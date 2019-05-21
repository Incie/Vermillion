#pragma once

#include"../activity.h"
#include"Renderer/Render.h"

class RasterizeMain : public Activity 
{
public:
	RasterizeMain();
	~RasterizeMain();

	void Initialize();
	void Deinitialize();

	void Update(double delta);
	void Render();

private:
	RolfGL render;
};