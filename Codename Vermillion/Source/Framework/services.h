#pragma once

#include"pch.h"
#include"point.h"
#include"glm/glm.hpp"
#include"inputstate.h"

class Texture {
public:
	unsigned int textureId;
	int width;
	int height;
	unsigned int channels;
};


class TextureService {
public:
	virtual Texture LoadTexture(const std::string& relativePath) = 0;
	virtual void UnloadAll() = 0;
};


class TextService {
public:
	virtual float CalculateWidth(const std::string& text, unsigned int fontHeight) const = 0;
	virtual float Print(double x, double y, const std::string& text, unsigned int fontHeight, const Colorf& color, bool center = false, bool newline = false) const = 0;
	virtual void NewLine(unsigned int fontHeight) const = 0;
	virtual void PrintCenter(const double x, const double y, const std::string& text, unsigned int fontHeight, const Colorf& color) const = 0;
};


class InputService {
public:
	virtual void Update() = 0;

	virtual bool KeyDown(char keyCode) const = 0;
	virtual bool KeyOnce(char keyCode) const = 0;
	virtual bool KeyUp(char keyCode) const = 0;

	virtual glm::vec2 GetMousePosition() const = 0;
	virtual glm::vec2 GetMousePositionNormalized() const = 0;
	virtual glm::vec2 GetMouseDelta() const = 0;

	virtual bool GamePadButtonDown(GamePadButton button) const = 0;
	virtual bool GamePadButtonOnce(GamePadButton button) const = 0;
	virtual bool GamePadButtonUp(GamePadButton button) const = 0;

	virtual double GamePadTriggerState(bool right) const = 0;
	virtual glm::vec2 GamePadRightStick() const = 0;
	virtual glm::vec2 GamePadLeftStick() const = 0;
};