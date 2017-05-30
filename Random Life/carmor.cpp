#include "carmor.h"

Armor::Armor()
{
	type = ARMOR;
	Group = ARMORS;
	rating = SuccessRate = 0;
	ArmorSkill = NONE_CHAR_SKILL;
}

Armor::Armor(int Armor_skin, CHAR_SKILL Armor_Skill, int Pickup_price, float Pickup_weight, CHAR_CLASS Pickup_Class, PickupMalus Pickup_Malus, Enchant Pickup_Enchant, int Item_width, int Item_height, Str Object_Name) :
Pickup(Pickup_price, Pickup_weight, false, NONE_PICKUP_UNIQUE_ID, ARMOR, ARMORS, Pickup_Class, Pickup_Malus, Pickup_Enchant), Item(NULL, NULL, Item_width, Item_height, Object_Name)
{
	rating = SuccessRate = 0;
	skin = Armor_skin;
	ArmorSkill = Armor_Skill;
}

Armor::Armor(int Armor_rating, int Armor_skin, CHAR_SKILL Armor_Skill, int Pickup_price, float Pickup_weight, bool Pickup_summoned, PICKUP_UNIQUE_ID Pickup_UniqueID, CHAR_CLASS Pickup_Class, PickupMalus Pickup_Malus, Enchant Pickup_Enchant, int Item_width, int Item_height, Str Object_Name) :
Pickup(Pickup_price, Pickup_weight, Pickup_summoned, Pickup_UniqueID, ARMOR, ARMORS, Pickup_Class, Pickup_Malus, Pickup_Enchant), Item(NULL, NULL, Item_width, Item_height, Object_Name)
{
	rating = Armor_rating;
	SuccessRate = 0;
	skin = Armor_skin;
	ArmorSkill = Armor_Skill;
}

Armor::Armor(Armor &model) : Pickup(model), Item(model)
{
	rating = model.GetRating();
	SuccessRate = model.GetSuccessRate();
	ArmorSkill = model.GetArmorSkill();
}

void Armor::GenerateSuccessRate(int CharArmorLevel)
{
	int MaxLevel, penalty;

	MaxLevel = 10;

	if (CharArmorLevel <= MaxLevel)
		penalty = MaxLevel - CharArmorLevel;
	else
		penalty = 0;

	switch (ArmorSkill)
	{
	case LIGHT_ARMOR:
		SuccessRate = (MaxLevel - penalty) * rating / 2;
		break;
	case HEAVY_ARMOR:
		SuccessRate = (MaxLevel - penalty) * rating;
		break;
	}
}

void Armor::GenerateAni(int frame)
{
	switch (ArmorSkill)
	{
	case LIGHT_ARMOR:
		frame = 0;
		break;
	case HEAVY_ARMOR:
		frame = 1;
		break;
	}

	Pickup::GenerateAni(frame);
}

bool Armor::compare(Object *pObject)
{
	Armor *pArmor = dynamic_cast<Armor*>(pObject);

	if (pArmor)
		return ((Pickup::compare(pArmor) == true) && (ArmorSkill == pArmor->GetArmorSkill()));

	return false;
}

Str Armor::GetTypeDescription()
{
	switch (ArmorSkill)
	{
	case LIGHT_ARMOR:
		return "Armure légère";
	case HEAVY_ARMOR:
		return "Armure lourde";
	default:
		return "?";
	}
}
