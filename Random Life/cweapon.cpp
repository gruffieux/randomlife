#include "cweapon.h"
#include "../RLRes/RLRes/resource.h"

Weapon::Weapon()
{
	MinDamage = HTH_MIN_DAMAGE;
	MaxDamage = HTH_MAX_DAMAGE;
	stunning = transpiercing = false;
	CombatSkill = HAND_TO_HAND;
}

Weapon::Weapon(CHAR_COMBAT_SKILL Weapon_CombatSkill, int Pickup_price, float Pickup_weight, CHAR_CLASS Pickup_Class, PickupMalus Pickup_Malus, Enchant Pickup_Enchant, int Item_width, int Item_height, Str Object_Name) :
Pickup(Pickup_price, Pickup_weight, false, NONE_PICKUP_UNIQUE_ID, WEAPON, WEAPONS, Pickup_Class, Pickup_Malus, Pickup_Enchant), Item(NULL, NULL, Item_width, Item_height, Object_Name)
{
	MinDamage = MaxDamage = 0;
	stunning = transpiercing = false;
	CombatSkill = Weapon_CombatSkill;
}

Weapon::Weapon(int Weapon_MinDamage, int Weapon_MaxDamage, CHAR_COMBAT_SKILL Weapon_CombatSkill, int Pickup_price, float Pickup_weight, bool Pickup_summoned, PICKUP_UNIQUE_ID Pickup_UniqueID, CHAR_CLASS Pickup_Class, PickupMalus Pickup_Malus, Enchant Pickup_Enchant, int Item_width, int Item_height, Str Object_Name) :
Pickup(Pickup_price, Pickup_weight, Pickup_summoned, Pickup_UniqueID, WEAPON, WEAPONS, Pickup_Class, Pickup_Malus, Pickup_Enchant), Item(NULL, NULL, Item_width, Item_height, Object_Name)
{
	MinDamage = Weapon_MinDamage;
	MaxDamage = Weapon_MaxDamage;
	stunning = transpiercing = false;
	CombatSkill = Weapon_CombatSkill;
}

Weapon::Weapon(Weapon &model) : Pickup(model), Item(model)
{
	MinDamage = model.GetMinDamage();
	MaxDamage = model.GetMaxDamage();
	stunning = model.GetStunning();
	transpiercing = model.GetTranspiercing();
	CombatSkill = model.GetCombatSkill();
}

void Weapon::initAni()
{
	animations.RemoveAllElement(0, true);
	animations.Initialize(1);

	switch (CombatSkill)
	{
	case DAGGERS:
		addAni(0, IDB_DAGGERS, "", 1, 4, 14, RGB(255, 255, 255), 90);
		break;
	case MACES:
		switch (Class)
		{
		case MEDIUM:
			addAni(0, IDB_MD_MACE, "", 1, 7, 23, RGB(255, 255, 255), 90);
			break;
		case STRONG:
			addAni(0, IDB_ST_MACE, "", 1, 10, 25, RGB(255, 255, 255), 90);
			break;
		default:
			addAni(0, IDB_WK_MACE, "", 1, 5, 19, RGB(255, 255, 255), 90);
			break;
		}
		break;
	case SWORDS:
		addAni(0, IDB_SWORDS, "", 1, 7, 21, RGB(255, 255, 255), 90);
		break;
	case AXES:
		switch (Class)
		{
		case MEDIUM:
			addAni(0, IDB_MD_AXE, "", 1, 17, 23, RGB(255, 255, 255), 90);
			break;
		default:
			addAni(0, IDB_ST_AXE, "", 1, 25, 40, RGB(255, 255, 255), 90);
			break;
		}
		break;
	}
}

bool Weapon::compare(Object *pObject)
{
	Weapon *pWeapon = dynamic_cast<Weapon*>(pObject);

	if (pWeapon)
		return (Pickup::compare(pWeapon) && CombatSkill == pWeapon->GetCombatSkill());

	return false;
}
