#include"pch/pch.h"
#include "PlayerRound.h"
#include"GL/glew.h"
#include"services.h"
#include"../action/Action.h"


PlayerRound::PlayerRound(std::vector<Action*>& actions)
	: currentAction(0)
{
	for (auto action : actions)
		this->actions.push_back(action);
}

PlayerRound::~PlayerRound()
{
	for (auto action : actions)
		delete action;
	actions.clear();
}

Action* PlayerRound::GetAction()
{
	if (currentAction < 0 || currentAction >= actions.size())
		return nullptr;

	return actions[currentAction];
}

void PlayerRound::Next()
{
	currentAction++;
}

bool PlayerRound::Finished()
{
	return (currentAction >= actions.size() || currentAction < 0);
}

void PlayerRound::Render(const TextService& text) const
{
	for( int i = 0; i < actions.size(); ++i ){
		auto action = actions[i];

		auto color = Colors::White;
		if (i == currentAction)
			color = Colors::Yellow;

		text.Print(0, 0, action->Description(), 20, color, false, true);
	}
}
