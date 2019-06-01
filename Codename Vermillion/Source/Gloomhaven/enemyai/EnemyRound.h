#pragma once

#include<vector>
#include<string>

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
	void RenderRoundCard(const TextService& text);

	enum class State {
		Stopped, Inprogress, Calculated, Finished
	};

	void ToString(std::vector<std::string>&);

	State state;

private:
	int currentAction;
	std::vector<EnemyAction*> actions;
};