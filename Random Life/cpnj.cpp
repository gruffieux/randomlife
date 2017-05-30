#include "cpnj.h"

Pnj::Pnj(CHAR_CLASS Char_Class, CHAR_PNJ_JOB Char_PNJJob, CHAR_PNJ_QUEST Char_PNJQUest, bool CharB_dispatch, bool CharB_GetPickup, CHAR_SKILL CharB_FavoriteSkill1, CHAR_SKILL CharB_FavoriteSkill2, CHAR_COMBAT_SKILL CharB_FavoriteCombatSkill, Str Object_Name) :
Item(NULL, NULL, NULL, NULL, Object_Name)
{
	PNJJob = Char_PNJJob;
	PNJQuest = Char_PNJQUest;
	Race = HUMANIZED;
	Class = Char_Class;
	Behavior.dispatch = CharB_dispatch;
	Behavior.GetPickup = CharB_GetPickup;
	Behavior.move = true;
	Behavior.FavoriteSkill1 = CharB_FavoriteSkill1;
	Behavior.FavoriteSkill2 = CharB_FavoriteSkill2;
	Behavior.FavoriteCombatSkill = CharB_FavoriteCombatSkill;
}

Pnj::Pnj(Pnj &model)
{
	PNJJob = model.GetPNJJob();
	PNJQuest = model.GetPNJQuest();
}

bool Pnj::CheckDest(Object *pObject)
{
	Character *pCharacter = dynamic_cast<Character*>(pObject);

	if (!pCharacter) return false;
	if (pCharacter->getRemoved()) return false;
	if ((pCharacter->GetStatus()->cursed == true) && (Status.cursed == true)) return false;
	if (pCharacter->GetStatus()->invisible) return false;
	if (pCharacter->GetStatus()->dead) return false;
	if (pCharacter->GetIgnoredBy()->SearchElement(this) != -1) return false;

	return TileIA::CheckDest(pCharacter);
}

void Pnj::CheckEquipment(MsgReporter *pMsgReporter)
{
	int i;
	CharSkill *BestSkill;
	Weapon *BestWeapon;
	Armor *BestArmor;

	if (!Status.dead)
	{
		if ((EquipedWp == NULL) && (Pocket[WEAPONS].GetElementCount() > 0))
		{
			BestSkill = CombatSkillPack.GetBestCombatSkill();
			BestWeapon = NULL;
			//Priorité 1: Une arme de classe PNJ correspondant à la meilleure compétence de combat
			for (i = 0; i < Pocket[WEAPONS].GetElementCount(); i++)
				if ((Weapon::getWeaponElement(i, &Pocket[WEAPONS])->GetClass() == Class) && (Weapon::getWeaponElement(i, &Pocket[WEAPONS])->GetCombatSkill() == BestSkill->GetSkillId()))
				{
					BestWeapon = Weapon::getWeaponElement(i, &Pocket[WEAPONS]);
					break;
				}
			if (BestWeapon)
			{
				//Priorité 2: Une arme de classe PNJ correspondant à la meilleure compétence de combat et des dommages maxi
				for (i = 0; i < Pocket[WEAPONS].GetElementCount(); i++)
					if ((Weapon::getWeaponElement(i, &Pocket[WEAPONS])->GetClass() == Class) && (Weapon::getWeaponElement(i, &Pocket[WEAPONS])->GetCombatSkill() == BestSkill->GetSkillId()) && (Weapon::getWeaponElement(i, &Pocket[WEAPONS])->GetMinDamage() > BestWeapon->GetMinDamage()))
						BestWeapon = Weapon::getWeaponElement(i, &Pocket[WEAPONS]);
				Equip(BestWeapon, pMsgReporter);
			}
		}
		if ((EquipedArmor == NULL) && (Pocket[ARMORS].GetElementCount() > 0))
		{
			BestSkill = SkillPack.GetBestArmorSkill();
			BestArmor = NULL;
			//Priorité 1: Une armure de classe PNJ correspondant à la meilleure compétence
			for (i = 0; i < Pocket[ARMORS].GetElementCount(); i++)
				if ((Armor::getArmorElement(i, &Pocket[ARMORS])->GetClass() == Class) && (Armor::getArmorElement(i, &Pocket[ARMORS])->GetArmorSkill() == BestSkill->GetSkillId()))
				{
					BestArmor = Armor::getArmorElement(i, &Pocket[ARMORS]);
					break;
				}
			//Priorité 2: Une armure de classe PNJ correspondant à la meilleure compétence avec un taux maxi
			if (BestArmor)
			{
				for (i = 0; i < Pocket[ARMORS].GetElementCount(); i++)
					if ((Armor::getArmorElement(i, &Pocket[ARMORS])->GetClass() == Class) && (Armor::getArmorElement(i, &Pocket[ARMORS])->GetArmorSkill() == BestSkill->GetSkillId()) && (Armor::getArmorElement(i, &Pocket[ARMORS])->GetRating() > BestArmor->GetRating()))
						BestArmor = Armor::getArmorElement(i, &Pocket[ARMORS]);
				Equip(BestArmor, pMsgReporter);
			}
		}
	}
}

EVENT_TYPE Pnj::Give(Pickup *pPickup, Humanized *pHumanized, MsgReporter *pMsgReporter, MiniEvents *pMiniEvents)
{
	EVENT_TYPE Result;

	Result = Humanized::Give(pPickup, pHumanized, pMsgReporter, pMiniEvents);

	CheckEquipment(pMsgReporter);

	return Result;
}

bool Pnj::Attack()
{
	if (CanAttack(Target)) return Humanized::Attack();
	return false;
}

void Pnj::Curse(bool value, short chao)
{
	if (!chao)
		Status.cursed = false;
	else
		Status.cursed = value;

	Humanized::Curse(Status.cursed, chao);
}

void Pnj::WinCombat(Object *Corpse, MsgReporter *pMsgReporter, MiniEvents *pMiniEvents)
{
	Humanized::WinCombat(Corpse, pMsgReporter, pMiniEvents);
	Mob::WinCombat(Corpse, pMsgReporter, pMiniEvents);
}

void Pnj::Diiiiiie(int ZoneCoordX, int ZoneCoordY, MsgReporter *pMsgReporter, MiniEvents *pMiniEvents)
{
	Humanized::Diiiiiie(pMsgReporter, pMiniEvents);
	Mob::Diiiiiie(ZoneCoordX, ZoneCoordY, pMsgReporter, pMiniEvents);
}
