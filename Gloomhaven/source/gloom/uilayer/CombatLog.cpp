#include"pch/pch.h"
#include "CombatLog.h"
#include"servicelocator.h"

#include"..//level/Level.h"

CombatLog::CombatLog(Level& level)
	: level(level)
{
	SetAnchor(WindowAnchor::BOTTOM | WindowAnchor::LEFT | WindowAnchor::RIGHT | WindowAnchor::TOP);
}

CombatLog::~CombatLog()
{
}

void CombatLog::Resize(const glm::vec2& windowSize, const TextService& text)
{
	UIView::Resize(windowSize, text);
}

void CombatLog::Measure(const glm::vec2& windowSize, const TextService& text)
{
	float maxWidth = 200.0f;
	for(const auto& s : level.combatLog ) {
		maxWidth = std::max<float>(text.CalculateWidth(s, 16), maxWidth);
	}

	size.x = maxWidth + 16.0f;
	size.y = windowSize.y;
}

#include"constants.h"
#include"GL/glew.h"
void CombatLog::Render(ServiceLocator& Services)
{
	if(!active)
		return;

	UIView::Render(Services);


	if(level.combatLog.size() == 0)
		return;

	float lineFit = size.y / static_cast<float>(level.combatLog.size());

	//todo: implement

	auto& text = Services.Text();
	glPushMatrix();
	for(const auto& s : level.combatLog) {
		text.Print(8, 8, s, 16, Colors::Black, false, true);
	}
	glPopMatrix();
}
