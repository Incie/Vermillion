#pragma once

#include"glm/vec2.hpp"


class WindowState
{
public:
	static void SetWindowState(int x, int y, int width, int height) {
		windowPosition = glm::vec2(x, y);
		windowSize = glm::vec2(width, height);
		hasChanged = true;
	}

	static glm::ivec2 Size() { return windowSize; }
	static int Width() { return windowSize.x; }
	static int Height() { return windowSize.y; }

	static bool Changed() { return hasChanged; }
	static void ResetChanged() { hasChanged = false; }

private:
	friend class InputState;

	static bool hasChanged;
	static glm::ivec2 windowPosition;
	static glm::ivec2 windowSize;
};