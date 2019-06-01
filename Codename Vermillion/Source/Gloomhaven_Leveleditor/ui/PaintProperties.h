#pragma once

#include"..//uilayer.h"
#include<functional>

class PaintProperties : public UILayer {
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

	virtual UILayerId LayerId() override;
private:
	std::function<void(int)> callback;
};