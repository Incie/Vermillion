#pragma once

#include"../activity.h"

class RasterizeMain : Activity 
{
public:
	RasterizeMain();
	~RasterizeMain();

	void Initialize();
	void Deinitialize();

	void Update(double delta);
	void Render();

private:

};