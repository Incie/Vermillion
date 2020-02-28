#include"pch/pch.h"
#include"menutest.h"


class VerticalWindow : public UIView {
public:
	VerticalWindow(std::function<void(int)> callback):callback(callback) {}
	virtual ~VerticalWindow(){}

	void Measure(const glm::vec2& windowSize, const TextService& text) {
		glm::vec2 size(0, 0);
		for(auto view : children) {
			view->Measure(text);
			size.x += view->Size().x + view->Margin().y * 2.0f;
			size.y += view->Size().y + view->Margin().y * 2.0f;
		}

		this->size = size;
		Log::Error("VW", fmt::format("{}, {}", this->size.x, this->size.y));
	}

	void Resize(const glm::vec2& windowSize, const TextService& text) override {
		UIView::Resize(windowSize, text);

		glm::vec2 position(0, 0);
		for(auto view : children) {
			view->SetPosition(position);
			position.y += view->Size().y;
		}

		Log::Error("VW", fmt::format("{}, {}", position.x, position.y));
	}

	void OnEvent(WindowEvent windowEvent, int id) override {
		if(windowEvent == WindowEvent::CLICK) {
			callback(id);
		}
	}
private:
	std::function<void(int)> callback;
};


MenuTest::MenuTest()
{
}

void MenuTest::Initialize()
{
	auto vw = new VerticalWindow([](int id) {
		throw std::string(fmt::format("Clicked {}", id));
	});
	vw->SetPosition(glm::vec2(50, 100));

	auto title = new UIText("Yolo Baggins");
	title->FontHeight(46.0f);
	title->SetSize(100, 26);
	vw->AddChild(title);

	auto buttonStart = new Button("Start", 26, 1);
	buttonStart->SetSize(100, 26);
	buttonStart->SetColor(glm::vec3(1, 0, 0));
	buttonStart->SetTextColor(glm::vec3(0, 0, 0));
	vw->AddChild(buttonStart);

	auto buttonExit = new Button("Exit", 26, 9);
	buttonExit->SetSize(100, 26);
	vw->AddChild(buttonExit);

	AddView(vw, 1);
}

void MenuTest::Deinitialize()
{
	DeinitializeUI();
}

void MenuTest::Update(float delta)
{
	UpdateUI(delta);
}

void MenuTest::Render()
{
	RenderUI();
}
