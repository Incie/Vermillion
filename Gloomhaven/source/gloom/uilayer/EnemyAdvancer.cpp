#include"pch/pch.h"
#include "EnemyAdvancer.h"
#include"..//icons/icons.h"
#include"render.h"
#include"textures.h"
#include"ui/uibutton.h"

EnemyAdvancer::EnemyAdvancer(std::function<void()> callback)
	: callback(callback), texture(nullptr), currentAction(0)
{
	SetSize(100, 100);
	SetAnchor(WindowAnchor::BOTTOM | WindowAnchor::RIGHT);
	
	auto button = vnew UIButton();
	button->SetText("Advance Enemy");
	button->SetPosition(8, 8);
	button->SetSize(150, 25);
	button->SetTextSize(18);
	children.push_back(button);

	texture = Icons::Get("monsterabilitycardback");
	
	Deactivate();
}

EnemyAdvancer::~EnemyAdvancer()
{
}

bool EnemyAdvancer::HandleInput(const InputService& inputService)
{
	if (!UIView::HandleInput(inputService))
		return false;

	return true;
}

void EnemyAdvancer::Resize(const glm::vec2& windowSize, const TextService& text)
{
	UIView::Resize(windowSize, text);

	if( children.size() > 0 )
		children[0]->SetPosition(size.x * 0.5f - children[0]->Size().x * 0.5f, size.y - children[0]->Size().y - 8);
}

void EnemyAdvancer::Measure(const glm::vec2& windowSize, const TextService& text)
{
	if(texture == nullptr)
		return;

	float desiredWidth = 300.0f;
	float scale = desiredWidth / static_cast<float>(texture->width);

	size.x = desiredWidth + 8 + 8;
	size.y = 8 + 25 + 8 + static_cast<float>(texture->height) * scale + 8;
}

#include"servicelocator.h"
void EnemyAdvancer::Render(ServiceLocator& Services)
{
	UIView::Render(Services);

	float desiredWidth = 300.0f;
	float scale = desiredWidth / static_cast<float>(texture->width);
	Render::Quad(*texture, static_cast<float>(texture->height) * scale);

	Services.Text().Print(30, 50, fmt::format("{0}", initiative), static_cast<unsigned int>(20), Colors::White, true);

	float textX = 8 + desiredWidth * 0.5f;
	float textY = 8 + 40.0f;
	float textSize = 18.0f;
	for(auto i = 0; i < actions.size(); ++i ) {
		const auto& action = actions[i];
		auto textColor = Colors::White;
		if(i == currentAction)
			textColor = glm::vec3(1, 1, 0);

		Services.Text().Print(textX, textY, action, static_cast<unsigned int>(textSize), textColor, true);
		textY += textSize + 2.0f;
	}
}

void EnemyAdvancer::SetEnemyActions(const std::vector<std::string>& actions, int initiative)
{
	this->initiative = initiative;
	this->actions.clear();
	this->actions = actions;
	currentAction = -1;
}

void EnemyAdvancer::Advance()
{
	currentAction++;
}


void EnemyAdvancer::OnEvent(WindowEvent type, int id)
{
	callback();
}
