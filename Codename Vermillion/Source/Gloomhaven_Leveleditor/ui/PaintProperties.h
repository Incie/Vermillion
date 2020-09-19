#pragma once

#include"..//uiview.h"
#include<functional>

class PaintProperties : public UIView {
public:
	PaintProperties(std::function<void(int)> callback);
	~PaintProperties();

	virtual void Resize(const glm::vec2& windowSize, const TextService& text);
	virtual void Measure(const glm::vec2& windowSize, const TextService& text);

	virtual void OnEvent(WindowEvent type, int id);


	enum ButtonId {
		ROOM_1 = 1,
		ROOM_2 = 2,
		ROOM_3 = 3,
		ROOM_4 = 4,
		ROOM_5 = 5
	};

private:
	std::function<void(int)> callback;
};