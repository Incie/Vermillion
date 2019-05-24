#include"pch.h"
#include "Modifiers.h"
#include <random>
#include <algorithm>
#include <iterator>
#include <numeric>

Modifiers::Modifiers()
	: needShuffle(false), lastDraw(-1)
{
	deck.reserve(20);
	discarded.reserve(20);

	deck.push_back(-10);
	deck.push_back(10);
	deck.push_back(0);
	deck.push_back(0);
	deck.push_back(0);
	deck.push_back(0);
	deck.push_back(0);
	deck.push_back(0);
	deck.push_back(-1);
	deck.push_back(-1);
	deck.push_back(-1);
	deck.push_back(-1);
	deck.push_back(-1);
	deck.push_back(1);
	deck.push_back(1);
	deck.push_back(1);
	deck.push_back(1);
	deck.push_back(1);
	deck.push_back(2);
	deck.push_back(-2);

	Shuffle();
}

Modifiers::~Modifiers()
{
}

int Modifiers::Draw()
{
	if (deck.size() == 0)
		Shuffle();

	assert(deck.size() != 0);

	int lastDraw = deck.back();
	deck.pop_back();
	discarded.push_back(lastDraw);

	if (lastDraw == -10 || lastDraw == 10)
		needShuffle = true;

	return lastDraw;
}

int Modifiers::GetLastDraw()
{
	return lastDraw;
}

void Modifiers::Shuffle()
{
	deck.insert(deck.end(), discarded.begin(), discarded.end());
	discarded.clear();

	std::random_device rnd;
	std::mt19937_64 gen(rnd());

	std::shuffle(deck.begin(), deck.end(), gen);
}

bool Modifiers::MarkedForShuffle()
{
	return needShuffle;
}

void Modifiers::Add(int n, int card)
{
	for(int i = 0; i < n; ++i)
		deck.push_back(card);
}

void Modifiers::Remove(int n, int card)
{
	throw "Modifiers::Remove() -> not implemented";
}
