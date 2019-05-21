#pragma once

#include"Buffer.h"

class RolfGL 
{
public:
	RolfGL();
	~RolfGL();

	void Draw();

private:
	Buffer frameBuffer;
	char c;
};