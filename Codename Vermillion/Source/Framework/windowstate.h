#pragma once


#include"point.h"


class WindowState
{
public:
	static void SetWindowState(int x, int y, int width, int height) {
		windowPosition.Set(x, y);
		windowSize.Set(width, height);
	}

private:
	friend class InputState;

	static Pointi windowPosition;
	static Pointi windowSize;
};