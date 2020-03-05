#pragma once


#include"ui/uiview.h"
#include<vector>
#include<string>

class Level;

class CombatLog : public UIView {
public:
	CombatLog(Level&);
	virtual ~CombatLog() override;

	virtual void Resize(const glm::vec2& windowSize, const TextService& text) override;
	virtual void Measure(const glm::vec2& windowSize, const TextService& text) override;
	virtual void Render(ServiceLocator& Services) override;
protected:
	Level& level;
};