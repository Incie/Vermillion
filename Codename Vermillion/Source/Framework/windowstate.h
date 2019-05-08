#pragma once

#include"point.h"

class WindowState
{
public:
	static void SetWindowState(int x, int y, int width, int height) {
		windowPosition.Set(x, y);
		windowSize.Set(width, height);
		hasChanged = true;
	}

	static Pointi Size() { return windowSize; }
	static int Width() { return windowSize.x; }
	static int Height() { return windowSize.y; }

	static bool Changed() { return hasChanged; }
	static void ResetChanged() { hasChanged = false; }

private:
	friend class InputState;

	static bool hasChanged;
	static Pointi windowPosition;
	static Pointi windowSize;
};