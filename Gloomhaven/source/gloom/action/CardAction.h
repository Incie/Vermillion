#pragma once


#include<string>
#include<vector>

class CardAction {
public:
	CardAction(const std::string& parseText);

	std::string action;
	int value;
	std::vector<CardAction> modifiers;
};
