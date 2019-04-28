#include"pch.h"
#include"EnemyRound.h"
#include"EnemyAction.h"
#include"../../Framework/services.h"

EnemyRound::EnemyRound()
	: currentAction(-1), state(State::Stopped)
{
}

EnemyRound::~EnemyRound()
{
	for (auto action : actions) {
		delete action;
	}
	actions.clear();
}

void EnemyRound::AddAction(EnemyAction* action)
{
	actions.push_back(action);
}

void EnemyRound::NextAction()
{
	currentAction++;
}

bool EnemyRound::HasNextAction()
{
	return (currentAction + 1 < (int)actions.size());
}

void EnemyRound::Reset()
{

	for (auto action : actions)
		action->Reset();
	currentAction = -1;
}

EnemyAction* EnemyRound::GetAction()
{
	if (currentAction >= 0 && currentAction < actions.size())
		return actions[currentAction];
	return nullptr;
}

void EnemyRound::Render()
{
	if (currentAction >= 0 && currentAction < actions.size()) {
		EnemyAction* action = actions[currentAction];
		if (state == State::Calculated)
			action->Render();
	}
}

void EnemyRound::RenderRoundCard(const TextService & text)
{
	auto white = Colorf(1.0f);
	auto active = Colorf(0.9f, 0.5f, 0.6f);
	int index = 0;
	for (auto action : actions) {
		auto& color = index == currentAction ? active : white;
		text.Print(0, 0, action->ActionDescription(), 18, color);
		index++;
	}
}