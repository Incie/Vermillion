#pragma once

class InputState;

class InputManager
{
public:
	InputManager();
	~InputManager();

	void Update();
	void SetWindowBounds(int x, int y, int width, int height);

	bool KeyDown(char keyCode);
	bool KeyOnce(char keyCode);
	bool KeyUp(char keyCode);
private:
	void UpdateKeyboard();
	void UpdateMouse();
	void UpdateGamepad();

	unsigned char keyStates[256];

	class Point {
	public:
		Point() { x = y = 0; }
		void Set(int x, int y) { this->x = x; this->y = y; }
		void Set(Point &p) { x = p.x; y = p.y; }

		int x, y;
	};
public:
	Point mousePosition;
	Point lastMousePosition;
	Point deltaMousePosition;

	float gpX, gpY;
	bool gpA;

	Point windowPosition;
	Point windowSize;
};


class InputState
{
public:
	static void SetKeyDown(char keyCode) {
		keyboardStates[keyCode] = 1;
	}
	static void SetKeyUp(char keyCode) {
		keyboardStates[keyCode] = 0;
	}
	
	static void Reset();

private:
	friend InputManager;
	static unsigned char keyboardStates[256];
};