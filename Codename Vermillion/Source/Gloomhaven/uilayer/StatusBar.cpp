#include"pch.h"
#include"..//icons/icons.h"
#include"StatusBar.h"
#include"../servicelocator.h"

StatusBar::StatusBar()
	: roundNumber(1)
{
	SetAnchor(UILayer::WindowAnchor::TOP | UILayer::WindowAnchor::LEFT | UILayer::WindowAnchor::RIGHT);
	SetSize(0, 30.0f);
	SetPosition(0, 0);

	auto imageSize = glm::vec2(30, 30);
	AddChild(new Image(imageSize, Icons::Get("element_ice")));
	AddChild(new Image(imageSize, Icons::Get("element_fire")));
	AddChild(new Image(imageSize, Icons::Get("element_earth")));
	AddChild(new UIText("Round 1"));
	AddChild(new Image(imageSize, Icons::Get("element_dark")));
	AddChild(new Image(imageSize, Icons::Get("element_light")));
	AddChild(new Image(imageSize, Icons::Get("element_wind")));

	int i = 0;
	for(auto child : children) {
		auto imageChild = dynamic_cast<Image*>(child);
		if(imageChild == nullptr)
			continue;
		
		imageChild->SetBrightness(0.3f);
	}
}

StatusBar::~StatusBar()
{
}

bool StatusBar::HandleInput(const InputService& inputService)
{
	return false;
}

void StatusBar::Resize(const glm::vec2& windowSize, const TextService& text)
{
	UILayer::Resize(windowSize, text);

	float x = 0;
	auto center = Center();

	for(auto child : children) {
		auto& margin = child->Margin();
		auto& size = child->Size();

		child->SetPosition(x + margin.x, margin.y);

		x += margin.x + margin.x + size.x;
	}
}

void StatusBar::Measure(const glm::vec2& dimensions, const TextService& text)
{
	float w = 0.0f;
	float h = 0.0f;

	for(auto child : children) {
		auto childSize = child->Measure(text);
		w += childSize.x;
		h = std::max<float>(h, childSize.y);
	}

	size.x = w;
	size.y = std::max<float>(h, 40);
}

void StatusBar::Render(ServiceLocator& Services)
{
	UILayer::Render(Services);
	float centerX = size.x * 0.5f;
	//Services.Text().PrintCenter(centerX, 8 + 15.0, fmt::format("Round {0}", roundNumber), 18, Colorf(0,0,0));
	Services.Text().PrintCenter(centerX, 16+15+9.0, statusText, 14, Colorf(0,0,0));
}

UILayerId StatusBar::LayerId()
{
	return UILayerId();
}
