#pragma once

#include<vector>

class Modifiers
{
public:
	Modifiers();
	~Modifiers();

	int Draw();
	int GetLastDraw();

	void Shuffle();
	void MarkedForShuffle();

	void Add(int n, int card);
	void Remove(int n, int card);

private:
	bool needShuffle;

	std::vector<int> discarded;
	std::vector<int> deck;
};