#pragma once

#include<vector>


enum class ModifierValue
{
	DoubleDamage,
	NoDamage,
	Zero,
	One,
	Two,
	Three,
	Four,
	MinusOne,
	MinusTwo
};

enum class ModifierStatus
{
	None,
	ElementFire,
	ElementEarth,
	ElementAir,
	ElementDark,
	ElementLight,
	ElementIce,
	ShieldSelf1,
	Pierce3,
	Push1,
	Stun,
	Wound,
	Poison,
	Disarm,
	Muddle,
	Strengthen,
	Target,
	Curse,
	Bless,
	Regenerate,
	Immobilize,
	Pull,
	Refresh,
	HealSelf,
	Invisible
};


class Modifier
{
public:
	constexpr Modifier(ModifierValue value, bool rolling = false, ModifierStatus status = ModifierStatus::None) 
		: value(value), rolling(rolling), status(status)
	{
	}

	int ModifyValue(int from);
	bool operator==(const ModifierValue& v) { return (value == v); }
	bool operator==(const Modifier& m) { return (m.value == value && m.status == status); }

	bool rolling;
	ModifierValue value;
	ModifierStatus status;

	static std::string StatusString(ModifierStatus status);
	static std::string ToString(const std::vector<Modifier>& modifiers);
};

class ModifierDeck
{
public:
	ModifierDeck();
	~ModifierDeck();

	std::vector<Modifier> Draw();

	void Shuffle();
	bool MarkedForShuffle();

	void Add(Modifier m);
	void Remove(Modifier m);

private:
	bool needShuffle;

	std::vector<Modifier> discarded;
	std::vector<Modifier> deck;
};