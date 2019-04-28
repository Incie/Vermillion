#pragma once

#include"point.h"

class WindowState
{
public:
	static void SetWindowState(int x, int y, int width, int height) {
		windowPosition.Set(x, y);
		windowSize.Set(width, height);
	}

	static Pointi Size() { return windowSize; }
	static int Width() { return windowSize.x; }
	static int Height() { return windowSize.y; }

private:
	friend class InputState;

	static Pointi windowPosition;
	static Pointi windowSize;
};