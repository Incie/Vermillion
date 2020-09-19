#pragma once

#include<vector>
#include<string>

class EnemyAction;
class TextService;
class Actor;

class EnemyRound {
public:
	EnemyRound();
	EnemyRound(int initiative, std::vector<EnemyAction*> actions, bool reshuffle = false);
	~EnemyRound();

	void AddAction(EnemyAction* action);
	void NextAction();
	bool HasNextAction();
	void Reset();
	EnemyAction* GetAction();

	void SetActorStats(const Actor& actor);

	void Render();
	void RenderRoundCard(const TextService& text);

	enum class RoundState {
		Stopped, Inprogress, Calculated, Finished
	};

	void ToString(std::vector<std::string>&);

	RoundState state;


	int Initiative() { return initiative; }
	bool Reshuffle() { return reshuffle; }
	void SetReshuffle() { reshuffle = true; }

private:
	bool reshuffle;
	int initiative;
	int currentAction;
	std::vector<EnemyAction*> actions;
};