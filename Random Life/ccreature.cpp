#include "ccreature.h"
#include "canimal.h"

Creature::Creature(CHAR_CLASS Char_Class, CHAR_BEHAVIOR Char_Behavior, bool CharB_CurseInfluenced, bool CharB_move, bool CharB_dispatch, bool CharB_GetPickup, CHAR_SKILL CharB_FavoriteSkill1, CHAR_SKILL CharB_FavoriteSkill2, CHAR_COMBAT_SKILL CharB_FavoriteCombatSkill, PICKUP_GROUP CharB_ThiefType, Str Object_Name) :
Item(NULL, NULL, NULL, NULL, Object_Name)
{
	Race = HUMANIZED;
	Class = Char_Class;
	PrimaryBehavior = Char_Behavior;
	Behavior.CurseInfluenced = CharB_CurseInfluenced;
	Behavior.move = CharB_move;
	Behavior.dispatch = CharB_dispatch;
	Behavior.GetPickup = CharB_GetPickup;
	Behavior.FavoriteSkill1 = CharB_FavoriteSkill1;
	Behavior.FavoriteSkill2 = CharB_FavoriteSkill2;
	Behavior.FavoriteCombatSkill = CharB_FavoriteCombatSkill;
	Behavior.ThiefType = CharB_ThiefType;
}

void Creature::CheckEquipment(MsgReporter *pMsgReporter)
{
	int i;
	CharSkill *BestSkill;
	Weapon *BestWeapon;
	Armor *BestArmor;

	if (!Status.dead)
	{
		for (i = 0; i < Pocket[OTHERS].GetElementCount(); i++)
			if ((Pickup::getPickupElement(i, &Pocket[OTHERS])->GetType() == JEWEL) && (Pickup::getPickupElement(i, &Pocket[OTHERS])->GetStatus()->equiped == false))
				Equip(Pickup::getPickupElement(i, &Pocket[OTHERS]), pMsgReporter);
		if (Pocket[WEAPONS].GetElementCount() > 0)
		{
			BestSkill = CombatSkillPack.GetBestCombatSkill();
			BestWeapon = NULL;
			//Priorité 1: Une arme dont la compétence de combat est la plus haute
			for (i = 0; i < Pocket[WEAPONS].GetElementCount(); i++)
				if (Weapon::getWeaponElement(i, &Pocket[WEAPONS])->GetCombatSkill() == BestSkill->GetSkillId())
				{
					BestWeapon = Weapon::getWeaponElement(i, &Pocket[WEAPONS]);
					break;
				}
			if (BestWeapon)
			{
				//Priorité 2: Une arme avec des dommages maxi, mais correspondant aussi à la compétence la plus haute
				for (i = 0; i < Pocket[WEAPONS].GetElementCount(); i++)
					if ((Weapon::getWeaponElement(i, &Pocket[WEAPONS])->GetCombatSkill() == BestSkill->GetSkillId()) && (Weapon::getWeaponElement(i, &Pocket[WEAPONS])->GetMinDamage() > BestWeapon->GetMinDamage()))
						BestWeapon = Weapon::getWeaponElement(i, &Pocket[WEAPONS]);
				//Priorité 3: Une arme avec enchantement
				for (i = 0; i < Pocket[WEAPONS].GetElementCount(); i++)
					if (Weapon::getWeaponElement(i, &Pocket[WEAPONS])->GetEnchant()->GetEffect() > BestWeapon->GetEnchant()->GetEffect())
						BestWeapon = Weapon::getWeaponElement(i, &Pocket[WEAPONS]);
				if (EquipedWp != BestWeapon)
					Equip(BestWeapon, pMsgReporter);
			}
		}
		if (Pocket[ARMORS].GetElementCount() > 0)
		{
			BestSkill = SkillPack.GetBestArmorSkill();
			BestArmor = NULL;
			//Priorité 1: Une armure dont la compétence est la plus haute
			for (i = 0; i < Pocket[ARMORS].GetElementCount(); i++)
				if (Armor::getArmorElement(i, &Pocket[ARMORS])->GetArmorSkill() == BestSkill->GetSkillId())
				{
					BestArmor = Armor::getArmorElement(i, &Pocket[ARMORS]);
					break;
				}
			if (BestArmor)
			{
				//Priorité 2: Une armure avec un taux de protection maxi, mais correspondant aussi à la compétence la plus haute
				for (i = 0; i < Pocket[ARMORS].GetElementCount(); i++)
					if ((Armor::getArmorElement(i, &Pocket[ARMORS])->GetArmorSkill() == BestSkill->GetSkillId()) && (Armor::getArmorElement(i, &Pocket[ARMORS])->GetRating() > BestArmor->GetRating()))
						BestArmor = Armor::getArmorElement(i, &Pocket[ARMORS]);
				//Priorité 3: Une armure avec enchantement
				for (i = 0; i < Pocket[ARMORS].GetElementCount(); i++)
					if (Armor::getArmorElement(i, &Pocket[ARMORS])->GetEnchant()->GetEffect() > BestArmor->GetEnchant()->GetEffect())
						BestArmor = Armor::getArmorElement(i, &Pocket[ARMORS]);
				if (EquipedArmor != BestArmor)
					Equip(BestArmor, pMsgReporter);
			}
		}
	}
}

bool Creature::CheckDest(Object *pObject)
{
	Character *pCharacter = dynamic_cast<Character*>(pObject);
	Animal *pAnimal = dynamic_cast<Animal*>(pObject);

	if (!pCharacter) return false;

	if (pAnimal)
	{
		if (Behavior.thief) return false;
		if (pAnimal->GetType() == MALEFIC_DOG) return false;
	}

	if (pCharacter->getRemoved()) return false;
	if ((pCharacter->GetStatus()->cursed == true) && (Status.cursed == true)) return false;
	if (pCharacter->GetStatus()->invisible) return false;
	if (pCharacter->GetStatus()->dead) return false;
	if (pCharacter->GetIgnoredBy()->SearchElement(this) != -1) return false;

	return TileIA::CheckDest(pCharacter);
}

EVENT_TYPE Creature::Give(Pickup *pPickup, Humanized *pHumanized, MsgReporter *pMsgReporter, MiniEvents *pMiniEvents)
{
	EVENT_TYPE Result;

	Result = Humanized::Give(pPickup, pHumanized, pMsgReporter, pMiniEvents);

	CheckEquipment(pMsgReporter);

	return Result;
}

bool Creature::Attack()
{
	if (CanAttack(Target)) return Humanized::Attack();
	return false;
}

bool Creature::LookForStealing()
{
	int i;
	Pickup *StealingPickup;

	if ((Behavior.thief == false) | (Status.fight == true) | (Target->GetStatus()->dead == true) | (HasNoTarget() == true))
		return false;

	if (Status.spy)
	{
		Status.spy = false;
		if (HumanizedTarget)
		{
			// Cherche un objet à voler
			for (i = 0; i < HumanizedTarget->GetPocket(Behavior.ThiefType)->GetElementCount(); i++)
			{
				StealingPickup = Pickup::getPickupElement(i, HumanizedTarget->GetPocket(Behavior.ThiefType));
				if ((StealingPickup->GetType() != MESSAGE) && (StealingPickup->GetSummoned() == false))
					if ((StealingPickup->GetClass() >= Class) | (StealingPickup->GetClass() == NONE_CHAR_CLASS))
					{
						SetStealing(StealingPickup);
						break;
					}
			}
			// N'a rien trouvé d'intéressant, ignore sa cible
			if (i == HumanizedTarget->GetPocket(Behavior.ThiefType)->GetElementCount())
			{
				Dests.RemoveElement(HumanizedTarget);
				HumanizedTarget->GetIgnoredBy()->AddElement(this, true);
				SetAsOwnTarget();
			}
		}
	}

	if (Stealing)
		if (GetAdjacent(Target))
		{
			Status.Action = NONE_CHAR_ACTION;
			if (!Swatch[BASIC].isGoing()) Steal(Stealing);
		}
		else
			Status.Action = MOVEMENT;

	return true;
}

bool Creature::MemorizeSpell(int school)
{
	int i;
	MagicSpell *pMagicSpell;
	MagicSpellList SpellPack;

	if (CurrentSpell.GetEffectDuration()->isGoing())
		return false;

	switch (school)
	{
	case CURE:
		pMagicSpell = MagicSpellPack[CURE-1].GetHigherMagicSpell(SELF_HEAL);
		if (pMagicSpell)
		{
			SetMemorizedSpell(pMagicSpell);
			return true;
		}
		return false;
	case DESTRUCTION:
		// On sélectionne tous les sorts à portée de la cible
		for (i = 0; i < MagicSpellPack[DESTRUCTION-1].GetElementCount(); i++)
			if (MagicSpellPack[DESTRUCTION-1].getMagicSpellElement(i)->GetRange()->data >= GetRange(Target->GetTileNodeID()))
				SpellPack.AddElement(MagicSpellPack[DESTRUCTION-1].getMagicSpellElement(i));
		// On cherche parmi la sélection le sort le plus haut niveau
		if ((HumanizedTarget != NULL) && (HumanizedTarget->GetManaPoints()->GetAmount() > 0))
		{
			pMagicSpell = SpellPack.GetHigherMagicSpell(ANTIMAGIC);
			if (!pMagicSpell) pMagicSpell = SpellPack.GetHigherMagicSpell(DESTRUCTION, 0, 0);
		}
		else
			pMagicSpell = SpellPack.GetHigherMagicSpell(DESTRUCTION, 0, ANTIMAGIC);
		// On mémorise le sort trouvé
		if ((pMagicSpell != NULL) && (CurrentSpell.GetEffectDuration()->getCounter() == 0))
		{
			SetMemorizedSpell(pMagicSpell);
			return true;
		}
		return false;
	case PROTECTION:
		pMagicSpell = MagicSpellPack[PROTECTION-1].GetHigherMagicSpell(PROTECTION, 0, 0);
		if ((pMagicSpell != NULL) && (CurrentSpell.GetEffectDuration()->getCounter() == 0))
		{
			SetMemorizedSpell(pMagicSpell);
			return true;
		}
		return false;
	default:
		return false;
	}
}

void Creature::WinCombat(Object *Corpse, MsgReporter *pMsgReporter, MiniEvents *pMiniEvents)
{
	Humanized::WinCombat(Corpse, pMsgReporter, pMiniEvents);
	Mob::WinCombat(Corpse, pMsgReporter, pMiniEvents);
}

void Creature::Diiiiiie(int ZoneCoordX, int ZoneCoordY, MsgReporter *pMsgReporter, MiniEvents *pMiniEvents)
{
	Humanized::Diiiiiie(pMsgReporter, pMiniEvents);
	Mob::Diiiiiie(ZoneCoordX, ZoneCoordY, pMsgReporter, pMiniEvents);
}
