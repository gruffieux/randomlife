#include "cpickup.h"
#include "../RLRes/RLRes/resource.h"

Pickup::Pickup()
{
	OwnerID = -1;
	price = 0;
	weight = 0;
	summoned = false;
	UniqueID = NONE_PICKUP_UNIQUE_ID;
	type = NONE_PICKUP_TYPE;
	Group = OTHERS;
	Class = NONE_CHAR_CLASS;
	DisplayedName = ReplaceCharByString(name, "0123456789", "#");
	DisplayedName = ReplaceStringByAnother(DisplayedName, " #", "");
}

Pickup::Pickup(int Pickup_price, float Pickup_weight, bool Pickup_summoned, PICKUP_UNIQUE_ID Pickup_UniqueID, PICKUP_TYPE Pickup_type, PICKUP_GROUP Pickup_Group, CHAR_CLASS Pickup_Class, PickupMalus Pickup_Malus, Enchant Pickup_Enchant)
{
	OwnerID = -1;
	price = Pickup_price;
	weight = Pickup_weight;
	summoned = Pickup_summoned;
	UniqueID = Pickup_UniqueID;
	type = Pickup_type;
	Group = Pickup_Group;
	Class = Pickup_Class;
	Malus = Pickup_Malus;
	PickupEnchant = Pickup_Enchant;
	DisplayedName = ReplaceCharByString(name, "0123456789", "#");
	DisplayedName = ReplaceStringByAnother(DisplayedName, " #", "");
}

Pickup::Pickup(int Pickup_price, float Pickup_weight, bool Pickup_summoned, PICKUP_UNIQUE_ID Pickup_UniqueID, PICKUP_TYPE Pickup_type, PICKUP_GROUP Pickup_Group, CHAR_CLASS Pickup_Class, PickupMalus Pickup_Malus, Enchant Pickup_Enchant, int Item_width, int Item_height, Str Object_Name) :
Item(NULL, NULL, Item_width, Item_height, Object_Name)
{
	OwnerID = -1;
	price = Pickup_price;
	weight = Pickup_weight;
	summoned = Pickup_summoned;
	UniqueID = Pickup_UniqueID;
	type = Pickup_type;
	Group = Pickup_Group;
	Class = Pickup_Class;
	Malus = Pickup_Malus;
	PickupEnchant = Pickup_Enchant;
	DisplayedName = ReplaceCharByString(name, "0123456789", "#");
	DisplayedName = ReplaceStringByAnother(DisplayedName, " #", "");
}

Pickup::Pickup(int Pickup_price, PICKUP_UNIQUE_ID Pickup_UniqueID, CHAR_CLASS Pickup_Class, int Item_width, int Item_height, Str Object_Name) :
Item(NULL, NULL, Item_width, Item_height, Object_Name)
{
	OwnerID = -1;
	price = Pickup_price;
	weight = price * 0.01f;
	summoned = false;
	UniqueID = Pickup_UniqueID;
	type = MONEY;
	Group = OTHERS;
	Class = Pickup_Class;
	DisplayedName = ReplaceCharByString(name, "0123456789", "#");
	DisplayedName = ReplaceStringByAnother(DisplayedName, " #", "");
}

Pickup::Pickup(Pickup &model) : TileActor(model), Item(model)
{
	OwnerID = model.GetOwnerID();
	price = model.GetPrice();
	weight = model.GetWeight();
	summoned = model.GetSummoned();
	UniqueID = model.GetUniqueID();
	type = model.GetType();
	Group = model.GetGroup();
	Class = model.GetClass();
	Malus = *model.GetMalus();
	Status = *model.GetStatus();
	PickupEnchant = *model.GetEnchant();
	DisplayedName = *model.GetDisplayedName();
}

void Pickup::initAni()
{
	animations.RemoveAllElement(0, true);
	animations.Initialize(1);

	switch (type)
	{
	case MONEY:
		addAni(0, IDB_MONEY, "", 1, 17, 17, RGB(255, 255, 255), 90);
		break;
	case MESSAGE:
		addAni(0, IDB_MESSAGE, "", 1, 7, 10, RGB(255, 255, 255), 90);
		break;
	case JEWEL:
		addAni(0, IDB_JEWELS, "", 2, 20, 20, RGB(255, 255, 255), 90);
		break;
	case POTION:
		addAni(0, IDB_POTIONS, "", 4, 6, 15, RGB(255, 255, 255), 90);
		break;
	case ARMOR:
		addAni(0, IDB_ARMORS, "", 2, 27, 21, RGB(255, 255, 255), 90);
		break;
	}
}

void Pickup::GenerateAni(int frame)
{
	switch (UniqueID)
	{
	case MINDPROTECTOR:
		frame = 0;
		break;
	case MAGICCIRCLE:
		frame = 1;
		break;
	}

	setVisibleZone();
	runAni(0, frame);
}

bool Pickup::compare(Object *pObject)
{
	Pickup *pPickup = dynamic_cast<Pickup*>(pObject);
	if (pPickup) return ((UniqueID == pPickup->GetUniqueID()) && (type == pPickup->GetType()) && (Class == pPickup->GetClass()));
	return false;
}

Str Pickup::GetTypeDescription()
{
	switch(type)
	{
	case MONEY:
		return "Argent";
	case MESSAGE:
		return "Message";
	case JEWEL:
		return "Bijou";
	case POTION:
		return "Potion";
	case WEAPON:
		return "Arme";
	default:
		return "Ingrédient";
	}
}
