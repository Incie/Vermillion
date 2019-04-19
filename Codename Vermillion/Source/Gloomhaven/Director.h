#pragma once

#include<vector>
#include<string>

class Level;

class Director {
public:
	Director(Level& level);
	~Director();


	void StartRound();
	void EndRound();


	void Render();
private:
	Level& level;
	std::vector<int> turnOrder;
	std::vector<std::string> turnOrderNames;
};