#include "cpotion.h"

Potion::Potion()
{
	int i;

	for (i = 0; i < TOTAL_PICKUP_POTION_ELEMENT; i++) ElementPower[i] = 0;
	power = 0;
	effect = NONE_PICKUP_POTION_EFFECT;
}

Potion::Potion(float Potion_power, PICKUP_POTION_EFFECT Potion_effect, int Pickup_price, float Pickup_weight, PICKUP_UNIQUE_ID Pickup_UniqueID, CHAR_CLASS Pickup_Class, int Item_width, int Item_height, Str Object_Name) :
Pickup(Pickup_price, Pickup_weight, false, Pickup_UniqueID, POTION, OTHERS, Pickup_Class, PickupMalus(), Enchant()), Item(NULL, NULL, Item_width, Item_height, Object_Name)
{
	int i;

	for (i = 0; i < TOTAL_PICKUP_POTION_ELEMENT; i++) ElementPower[i] = 0;
	power = Potion_power;
	effect = Potion_effect;
}

Potion::Potion(Potion &model) : Pickup(model), Item(model)
{
	int i;

	power = model.GetPower();
	for (i = 0; i < TOTAL_PICKUP_POTION_ELEMENT; i++)
		ElementPower[i] = model.GetElementPower(i);
	effect = model.GetEffect();
}

void Potion::GenerateAni(int frame)
{
	switch (effect)
	{
	case LIFE_REGEN:
		frame = 0;
		break;
	case MANA_REGEN:
		frame = 1;
		break;
	case MANA_TRANS_LIFE:
		frame = 2;
		break;
	case INVISIBILITY:
		frame = 3;
		break;
	}

	Pickup::GenerateAni(frame);
}

bool Potion::compare(Object *pObject)
{
	Potion *pPotion = dynamic_cast<Potion*>(pObject);

	if (pPotion)
		return (Pickup::compare(pPotion) && effect == pPotion->GetEffect());

	return false;
}

Str Potion::GetEffectDescription()
{
	switch (effect)
	{
	case LIFE_REGEN:
		return "Regain vie";
	case MANA_REGEN:
		return "Regain mana";
	case MANA_TRANS_LIFE:
		return "Mana pour vie";
	case INVISIBILITY:
		return "Invisibilité";
	default:
		return "Aucun";
	}
}
