#include"pch.h"
#include"ActionAttack.h"
#include"../level/Level.h"
#include"../entity/Entity.h"

ActionAttack::ActionAttack(Level& level, Actor& actor, int range, int attackDamage, int attacks, std::vector<StatusEffect> statusEffects)
	: Action(level, actor), range(range), attacks(attacks), baseDamage(attackDamage)
{
	this->statusEffects.insert(this->statusEffects.end(), statusEffects.begin(), statusEffects.end());
	actionDescription = fmt::format("Attack {0} Range {1} Target {2}", attackDamage, range, attacks);
	Highlight();
}

ActionAttack::~ActionAttack()
{
	marks.clear();
}

void ActionAttack::Click(const glm::ivec3& target)
{
	const Tile& tile = level.TileAt(target);

	if (tile.IsOccupied()) {

		if (IsMarked(tile.OccupiedId())){
			//todo RemoveMark
			return;
		}

		if (marks.size() >= attacks)
			return;

		if (tile.DistanceTo(target) > range)
			return;

		if (level.TileAt(actor.Position()).DistanceTo(target) == 0)
			return;

		marks.push_back(target);
		Highlight();
	}
}

void ActionAttack::Undo()
{
	if (marks.size() > 0)
		marks.pop_back();

	Highlight();
}

void ActionAttack::Reset()
{
	marks.clear();
	Highlight();
}

bool ActionAttack::Perform(Actor& actor)
{
	for (auto& mark : marks) {
		auto& tile = level.TileAt(mark);
		auto targetActor = level.ActorById(tile.OccupiedId());

		int poisonDamage = 0;
		if(targetActor->Poisoned())
			poisonDamage = 1;

		int calculatedDamage = baseDamage + poisonDamage;
		auto modifiers = level.playerModifiers.Draw();
		for(auto m : modifiers)
			calculatedDamage = m.ModifyValue(calculatedDamage);

		int pierce = actor.Pierce();

		for(auto status : statusEffects) {
			if(status == StatusEffect::Pierce) pierce += 1;
			if(status == StatusEffect::Pierce2) pierce += 2;
			if(status == StatusEffect::Pierce3) pierce += 3;
		}


		for(auto& m : modifiers) {
			if(m.status != ModifierStatus::None) {
				switch(m.status) {
					case ModifierStatus::ShieldSelf1: {
						actor.ModifyShield(1);
						actor.AddEndOfRoundAction([](Actor* a) { a->ModifyShield(-1); });
						break;
					}
					case ModifierStatus::Pierce3: {
						pierce += 3;
						break;
					}
					default: {
						switch(m.status) {
							case ModifierStatus::Poison:	  statusEffects.push_back(StatusEffect::Poison);	 break;
							case ModifierStatus::Wound: 	  statusEffects.push_back(StatusEffect::Wound);		 break;
							case ModifierStatus::Stun: 		  statusEffects.push_back(StatusEffect::Stunned);	 break;
							case ModifierStatus::Disarm:	  statusEffects.push_back(StatusEffect::Disarmed);	 break;
							case ModifierStatus::Immobilize:  statusEffects.push_back(StatusEffect::Immobilized);break;
							case ModifierStatus::Strengthen:  statusEffects.push_back(StatusEffect::Strengthen); break;
							case ModifierStatus::Invisible:	  statusEffects.push_back(StatusEffect::Invisible);	 break;
							case ModifierStatus::Muddle: 	  statusEffects.push_back(StatusEffect::Muddle);	 break;
							default: 
							break;
						}
						break;
					}
				}
			}
		}

		int actualDamage = targetActor->DoDamage(calculatedDamage, pierce);
		level.combatLog.push_back(fmt::format("{0} did {1} ({4} + {3}) damage to {2}", dynamic_cast<Player*>(&actor)->PlayerName(), actualDamage, targetActor->Name(), Modifier::ToString(modifiers), baseDamage));

		for(auto st : statusEffects) {
			switch(st) {
				case StatusEffect::Strengthen: actor.AddStatus(st); break;
				case StatusEffect::Invisible: actor.AddStatus (st); break;
				case StatusEffect::Pierce: 
				case StatusEffect::Pierce2:
				case StatusEffect::Pierce3:
					break;
				default:  {
					level.combatLog.push_back(fmt::format("[Target] gained {0}", StatusEffectToString(st)));
					targetActor->AddStatus(st);
					break;
				}
			}
		}
		


		if (targetActor->Health() <= 0) {
			level.combatLog.push_back("[Enemy] died");
			level.RemoveActorById(targetActor->EntityId());
			targetActor->Deactivate();
		}
	}

	return true;
}

#include"GL/glew.h"
void ActionAttack::Render()
{
	const auto& tile0 = level.TileAt(actor.Position());
	glBegin(GL_LINES);
	for (auto mark : marks) {
		glColor3f(1, 0, 0);

		const auto& tile1 = level.TileAt(mark);
		glVertex2fv(&tile0.WorldPosition().x);
		glVertex2fv(&tile1.WorldPosition().x);
	}
	glEnd();
}

void ActionAttack::Highlight()
{
	level.ClearHighlights();

	if (marks.size() >= attacks)
		return;

	level.Highlight(actor.Position(), range, glm::vec3(1, 0, 0), [this](const Tile & tile) {
		if (tile.DistanceTo(actor.Position()) == 0)
			return false;

		if (!tile.IsOccupied())
			return false;

		if (IsMarked(tile.OccupiedId()))
			return false;


		return true; 
	});
}

void ActionAttack::NoHighlight()
{
}

bool ActionAttack::IsMarked(int entityId)
{
	for (auto mark : marks)
	{
		const auto& tile = level.TileAt(mark);
		if (tile.OccupiedId() == entityId)
			return true;
	}

	return false;
}
