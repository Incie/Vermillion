#pragma once

#include<vector>

class TextService;
class Action;

class PlayerRound {
public:
	PlayerRound(std::vector<Action*>& actions);
	~PlayerRound();

	Action* GetAction();
	void Next();
	bool Finished();

	void Render(const TextService& text) const;

private:
	int currentAction;
	std::vector<Action*> actions;
};