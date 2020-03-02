#include"pch/pch.h"
#include"menutest.h"

namespace UI {
#define COLOR(name, r,g,b) glm::vec3 name = glm::vec3(r/255.0f,g/255.0f,b/255.0f)

	COLOR(Dark, 19, 15, 64);
	COLOR(Light, 223, 249, 251);
	COLOR(Positive, 106, 176, 76);
	COLOR(Negative, 235, 77, 75);
	COLOR(Neutral, 72, 52, 212);
};


class VerticalWindow : public UIView {
public:
	VerticalWindow(std::function<void(int)> callback):callback(callback) {}
	virtual ~VerticalWindow(){}

	void Measure(const glm::vec2& windowSize, const TextService& text) {
		glm::vec2 size(0, 0);
		for(auto view : children) {
			view->Measure(text);
			size.x = glm::max(view->Size().x + view->Margin().x * 2.0f, size.x);
			size.y += view->Size().y + view->Margin().y * 2.0f;
		}

		this->size = size;
		Log::Error("VW", fmt::format("{}, {}", this->size.x, this->size.y));
	}

	void Resize(const glm::vec2& windowSize, const TextService& text) override {
		UIView::Resize(windowSize, text);

		size.x = children[0]->Size().x + children[0]->Margin().x * 2.0f;

		glm::vec2 position(0, 0);
		for(auto view : children) {
			position.x = view->Margin().x;
			position.y += view->Margin().y;
			view->SetPosition(position);
			view->SetSize(size.x, view->Size().y);

			position.x = 0.0f;
			position.y += view->Size().y + view->Margin().y;
		}

		size.y = position.y;
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
	auto vw = new VerticalWindow([this](int id) {
		if(id == 1)
		{
			StartActivity("Sandbox");
			return;
		}

		if(id == 9) {
			Finish();
			return;
		}

		throw std::string(fmt::format("Clicked {}", id));
	});
	vw->BackgroundColor(UI::Dark);
	vw->SetPosition(glm::vec2(50, 100));

	auto title = new UIText("Game Title Here");
	title->Color(UI::Light);
	title->FontHeight(46.0f);
	title->SetSize(100, 26);
	title->SetMargin(4.0f, 8.0f);
	vw->AddChild(title);

	auto buttonStart = new Button("Start", 26, 1);
	buttonStart->SetSize(100, 26);
	buttonStart->SetColor(UI::Light);
	buttonStart->SetTextColor(UI::Dark);
	buttonStart->SetMargin(0, 2.0f);
	vw->AddChild(buttonStart);

	auto buttonExit = new Button("Exit", 26, 9);
	buttonExit->SetSize(100, 26);
	buttonExit->SetColor(UI::Negative);
	buttonExit->SetTextColor(UI::Dark);
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
