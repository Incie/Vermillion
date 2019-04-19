#pragma once

#include<vector>

class EnemyAction;
class TextService;

class EnemyRound {
public:
	EnemyRound();
	~EnemyRound();

	void AddAction(EnemyAction* action);
	void NextAction();
	bool HasNextAction();
	void Reset();
	EnemyAction* GetAction();

	void Render();
	void RenderRoundCard(TextService& text);

	enum class State {
		Stopped, Inprogress, Calculated, Finished
	};

	State state;

private:
	int currentAction;
	std::vector<EnemyAction*> actions;
};