#include "cpocket.h"
#include "clookedpickup.h"
#include "../RLRes/RLRes/resource.h"

extern Display *Screen;
extern LookedPickup *PPickup;

Pocket::Pocket(int Panel_ColSpace, int Panel_RowSpace, int Panel_BorderSpace, Str Object_Name) :
IconList(true, 6, Panel_ColSpace, Panel_RowSpace, Panel_BorderSpace, false, Str()),
Item(NULL, NULL, NULL, NULL, Object_Name)
{
	Focus = NULL;
}

Pocket::Pocket(int Panel_ColSpace, int Panel_RowSpace, int Panel_BorderSpace, int Item_width, int Item_height, Str Object_Name) :
IconList(true, 6, Panel_ColSpace, Panel_RowSpace, Panel_BorderSpace, false, NULL, NULL, Str()),
Item(NULL, NULL, Item_width, Item_height, Object_Name)
{
	Focus = NULL;
}

void Pocket::InitIcons(PICKUP_GROUP group)
{
	switch (group)
	{
	case WEAPONS:
		IconList::InitIcons(IDB_POCKET_WEAPONS, "", 8, 32, 32);
		break;
	case ARMORS:
		IconList::InitIcons(IDB_POCKET_ARMORS, "", 3, 32, 32);
		break;
	case INGREDIENTS:
		IconList::InitIcons(IDB_POCKET_INGREDIENTS, "", 12, 32, 32);
		break;
	default:
		IconList::InitIcons(IDB_POCKET_OTHERS, "", 9, 32, 32);
		break;
	}
}

bool Pocket::Run(Item *Cursor)
{
	if (Focus) return Panel::Run(Cursor);

	return false;
}

bool Pocket::Refresh()
{
	int i, PickupID;

	if (!Focus) return false;

	for (i = 1; i < RowCount; i++)
	{
		PickupID = i - 1 + Selector->GetStage(Focus->GetElementCount()-1);

		if (PickupID < Focus->GetElementCount())
		{
			// Frames pour les potions
			if (Potion::getPotionElement(PickupID, Focus))
			{
				switch (Potion::getPotionElement(PickupID, Focus)->GetEffect())
				{
				case LIFE_REGEN:
					Grid[i][1].PicFrame = 2;
					break;
				case MANA_REGEN:
					Grid[i][1].PicFrame = 3;
					break;
				case MANA_TRANS_LIFE:
					Grid[i][1].PicFrame = 4;
					break;
				case INVISIBILITY:
					Grid[i][1].PicFrame = 5;
					break;
				}
			}

			// Frames pour les armes
			if (Weapon::getWeaponElement(PickupID, Focus))
			{
				switch (Weapon::getWeaponElement(PickupID, Focus)->GetCombatSkill())
				{
				case DAGGERS:
					Grid[i][1].PicFrame = 1;
					break;
				case MACES:
					switch (Weapon::getWeaponElement(PickupID, Focus)->GetClass())
					{
					case WEAK:
						Grid[i][1].PicFrame = 2;
						break;
					case MEDIUM:
						Grid[i][1].PicFrame = 3;
						break;
					case STRONG:
						Grid[i][1].PicFrame = 4;
						break;
					default:
						Grid[i][1].PicFrame = 2;
						break;
					}
					break;
				case SWORDS:
					Grid[i][1].PicFrame = 5;
					break;
				case AXES:
					switch (Weapon::getWeaponElement(PickupID, Focus)->GetClass())
					{
					case MEDIUM:
						Grid[i][1].PicFrame = 6;
						break;
					case STRONG:
						Grid[i][1].PicFrame = 7;
						break;
					default:
						Grid[i][1].PicFrame = 7;
						break;
					}
					break;
				}
			}

			// Frames pour les armures
			if (Armor::getArmorElement(PickupID, Focus))
			{
				switch (Armor::getArmorElement(PickupID, Focus)->GetArmorSkill())
				{
				case LIGHT_ARMOR:
					Grid[i][1].PicFrame = 1;
					break;
				case HEAVY_ARMOR:
					Grid[i][1].PicFrame = 2;
					break;
				}
			}

			// Frames pour les ingrédients
			if (Ingredient::getIngredientElement(PickupID, Focus))
			{
				switch (Ingredient::getIngredientElement(PickupID, Focus)->GetIngredientType())
				{
				case FLOWER_WHITE:
					Grid[i][1].PicFrame = 1;
					break;
				case FLOWER_RED:
					Grid[i][1].PicFrame = 2;
					break;
				case FLOWER_BLUE:
					Grid[i][1].PicFrame = 3;
					break;
				case FRUIT_RED:
					Grid[i][1].PicFrame = 4;
					break;
				case FRUIT_PINK:
					Grid[i][1].PicFrame = 5;
					break;
				case FRUIT_BLUE:
					Grid[i][1].PicFrame = 6;
					break;
				case FRUIT_BLACK:
					Grid[i][1].PicFrame = 7;
					break;
				case FRUIT_ORANGE:
					Grid[i][1].PicFrame = 8;
					break;
				case DRINK_WATER:
					Grid[i][1].PicFrame = 9;
					break;
				case DRINK_WINE:
					Grid[i][1].PicFrame = 10;
					break;
				case DRINK_TROLLFLUID:
					Grid[i][1].PicFrame = 11;
					break;
				}
			}

			// Frames pour les objets divers
			switch (Pickup::getPickupElement(PickupID, Focus)->GetType())
			{
			case MONEY:
				Grid[i][1].PicFrame = 1;
				break;
			case MESSAGE:
				Grid[i][1].PicFrame = 8;
				break;
			case JEWEL:
				switch (Pickup::getPickupElement(PickupID, Focus)->GetUniqueID())
				{
				case MINDPROTECTOR:
					Grid[i][1].PicFrame = 6;
					break;
				case MAGICCIRCLE:
					Grid[i][1].PicFrame = 7;
					break;
				}
				break;
			}
		}
		else
			Grid[i][1].PicFrame = 0;
	}

	return IconList::Refresh();
}

bool Pocket::generate(bool waited, Cursor *cursor)
{	
	int i, PickupID;

	if (!Focus) return false;

	// Evénements sur chaque objet
	for (i = 1; i < RowCount; i++)
	{
		PickupID = i - 1 + Selector->GetStage(Focus->GetElementCount()-1);

		if ((ClickOnGrid(1, i, waited, cursor) == true) && (PickupID < Focus->GetElementCount()))
		{
			PPickup->SetFocus(Pickup::getPickupElement(PickupID, Focus));
			PPickup->show();
			PPickup->Refresh();
		}
	}

	return IconList::generate(waited, cursor);
}
