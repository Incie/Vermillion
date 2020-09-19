#include"pch.h"
#include "Modifiers.h"
#include <random>
#include <algorithm>
#include <iterator>
#include <numeric>

ModifierDeck::ModifierDeck()
	: needShuffle(false)
{
	deck.reserve(20);
	discarded.reserve(20);

	//Default Deck
	Add(Modifier(ModifierValue::NoDamage));
	Add(Modifier(ModifierValue::DoubleDamage));
	Add(Modifier(ModifierValue::Zero));
	Add(Modifier(ModifierValue::Zero));
	Add(Modifier(ModifierValue::Zero));
	Add(Modifier(ModifierValue::Zero));
	Add(Modifier(ModifierValue::Zero));
	Add(Modifier(ModifierValue::Zero));
	Add(Modifier(ModifierValue::MinusOne));
	Add(Modifier(ModifierValue::MinusOne));
	Add(Modifier(ModifierValue::MinusOne));
	Add(Modifier(ModifierValue::MinusOne));
	Add(Modifier(ModifierValue::MinusOne));
	Add(Modifier(ModifierValue::MinusTwo));
	Add(Modifier(ModifierValue::Two));
	Add(Modifier(ModifierValue::One));
	Add(Modifier(ModifierValue::One));
	Add(Modifier(ModifierValue::One));
	Add(Modifier(ModifierValue::One));
	Add(Modifier(ModifierValue::One));


	Shuffle();
}

ModifierDeck::~ModifierDeck()
{
}

void ModifierDeck::AddPerks() {
	// Perks
	Remove(Modifier(ModifierValue::MinusOne));
	Remove(Modifier(ModifierValue::MinusOne));

	Remove(Modifier(ModifierValue::MinusOne));
	Add(Modifier(ModifierValue::One));

	Add(Modifier(ModifierValue::Three));

	Add(Modifier(ModifierValue::Zero, true, ModifierStatus::Pierce3));
	Add(Modifier(ModifierValue::Zero, true, ModifierStatus::Pierce3));

	Add(Modifier(ModifierValue::Zero, true, ModifierStatus::Stun));

	Add(Modifier(ModifierValue::One, false, ModifierStatus::ShieldSelf1));

	Add(Modifier(ModifierValue::One, true));
	Add(Modifier(ModifierValue::One, true));
}

ModifierDraw ModifierDeck::Draw()
{
	ModifierDraw modifierDraw;

	while(1) 
	{
		if (deck.size() == 0)
			Shuffle();

		assert(deck.size() != 0);

		auto draw = deck.back();
		deck.pop_back();

		discarded.push_back(draw);
		modifierDraw.modifiers.push_back(draw);

		if (draw == ModifierValue::NoDamage || draw == ModifierValue::DoubleDamage )
			needShuffle = true;

		if(!draw.rolling)
			break;
	}

	return modifierDraw;
}

void ModifierDeck::Shuffle()
{
	deck.insert(deck.end(), discarded.begin(), discarded.end());
	discarded.clear();

	std::random_device rnd;
	std::mt19937_64 gen(rnd());

	std::shuffle(deck.begin(), deck.end(), gen);
}

bool ModifierDeck::MarkedForShuffle()
{
	return needShuffle;
}

void ModifierDeck::Add(Modifier m)
{
	deck.push_back(m);
}

void ModifierDeck::Remove(Modifier m)
{
	Shuffle();

	auto found = std::find_if(deck.begin(), deck.end(), [&m](Modifier modifier) { return m == modifier; });
	

	if(found == deck.end()) {
		throw "No Card Found To Remove in ModifierDeck::Remove";
	}

	deck.erase(found);
}

#include<sstream>
int Modifier::ModifyValue(int from)
{
	switch(value) {
		case ModifierValue::NoDamage:
			return 0;
		case ModifierValue::DoubleDamage:
			return from * 2;
		case ModifierValue::Zero:
			return from;
		case ModifierValue::One:
			return from + 1;
		case ModifierValue::Two:
			return from + 2;
		case ModifierValue::Three:
			return from + 3;
		case ModifierValue::Four:
			return from + 4;
		case ModifierValue::MinusOne:
			return from - 1;
		case ModifierValue::MinusTwo:
			return from - 1;
		default:
			throw "unknown modifiervalue in ModifyValue";
	}
}

std::string Modifier::StatusString(ModifierStatus status)
{
	switch(status) {
		case ModifierStatus::None:
		return "None";
		case ModifierStatus::ElementFire:
		return "Fire";
		case ModifierStatus::ElementEarth:
		return "Earth";
		case ModifierStatus::ElementAir:
		return "Air";
		case ModifierStatus::ElementDark:
		return "Dark";
		case ModifierStatus::ElementLight:
		return "Light";
		case ModifierStatus::ElementIce:
		return "Ice";
		case ModifierStatus::ShieldSelf1:
		return "Shield1Self";
		case ModifierStatus::Pierce3:
		return "Pierce3";
		case ModifierStatus::Push1:
		return "Push1";
		case ModifierStatus::Stun:
		return "Stun";
		case ModifierStatus::Wound:
		return "Wound";
		case ModifierStatus::Poison:
		return "Poison";
		case ModifierStatus::Disarm:
		return "Disarm";
		case ModifierStatus::Muddle:
		return "Muddle";
		case ModifierStatus::Target:
		return "Target";
		case ModifierStatus::Curse:
		return "Curse";
		case ModifierStatus::Regenerate:
		return "Regenerate";
		case ModifierStatus::Immobilize:
		return "Immobilize";
		case ModifierStatus::Pull:
		return "Pull";
		case ModifierStatus::Refresh:
		return "RefreshItem";
		case ModifierStatus::HealSelf:
		return "HealSelf";
		case ModifierStatus::Bless:
		return "Bless";
		case ModifierStatus::Strengthen:
		return "Strengthen";
		case ModifierStatus::Invisible:
		return "Invisible";
	}

	throw "Unknown status in StatusString";
}

std::string Modifier::ToString(const std::vector<Modifier>& modifiers)
{
	std::ostringstream ss;

	int modifierIndex = 0;
	for(auto modifier : modifiers) {
		if(modifierIndex != 0)
			ss << " + ";

		if(modifier.value == ModifierValue::NoDamage || modifier.status == ModifierStatus::Curse )
			ss << "[NoDamage]";
		else if(modifier.value == ModifierValue::DoubleDamage || modifier.status == ModifierStatus::Bless )
			ss << "x2!";
		else {
			if( modifierIndex == 0 || modifier.value != ModifierValue::Zero )
				ss << modifier.ModifyValue(0);
		}

		if(modifier.rolling)
			ss << " R ";

		if(modifier.status != ModifierStatus::None) {
			ss << " [" << Modifier::StatusString(modifier.status) << "]";
		}

		modifierIndex++;
	}

	return ss.str();
}

std::pair<int, int> ModifierDraw::TotalDamage(int baseDamage)
{
	auto damage = std::pair<int, int>(baseDamage, 0);

	for(auto modifier : modifiers) {
		damage.first = modifier.ModifyValue(damage.first);
		
		if(modifier.status != ModifierStatus::None)
			damage.second++;
	}

	return damage;
}
