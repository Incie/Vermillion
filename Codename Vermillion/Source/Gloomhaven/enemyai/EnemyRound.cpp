#include"pch.h"
#include"EnemyRound.h"
#include"actions/EnemyAction.h"
#include"../../Framework/services.h"

EnemyRound::EnemyRound()
	: currentAction(-1), state(RoundState::Stopped), initiative(0), reshuffle(false)
{
}

EnemyRound::EnemyRound(int initiative, std::vector<EnemyAction*> actions, bool reshuffle)
	: initiative(initiative), currentAction(-1), state(RoundState::Stopped), reshuffle(reshuffle)
{
	this->actions.insert(this->actions.begin(), actions.begin(), actions.end());
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

void EnemyRound::SetActorStats(const Actor& actor)
{
	for(auto action : actions) {
		action->Modify(actor);
	}
}

void EnemyRound::Render()
{
	if (currentAction >= 0 && currentAction < actions.size()) {
		EnemyAction* action = actions[currentAction];
		if (state == RoundState::Calculated)
			action->Render();
	}
}

void EnemyRound::RenderRoundCard(const TextService & text)
{
	auto active = glm::vec3(0.9f, 0.5f, 0.6f);
	int index = 0;
	for (auto action : actions) {
		auto& color = index == currentAction ? active : Colors::White;
		text.Print(0, 0, action->ActionDescription(), 18, color, false, true);
		index++;
	}
}

void EnemyRound::ToString(std::vector<std::string>&str)
{
	for(auto action : actions) {
		str.push_back(action->ActionDescription());
	}
}
