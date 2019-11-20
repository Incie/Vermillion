#pragma once

#include<vector>

class TextService;
class Action;

class PlayerRound {
public:
	PlayerRound() { currentAction = 0; }
	PlayerRound(std::vector<Action*>& actions);
	~PlayerRound();

	void AddAction(Action* action) { actions.push_back(action); }
	Action* GetAction();
	void Next();
	bool Finished();

	void Render(const TextService& text) const;

private:
	int currentAction;
	std::vector<Action*> actions;
};