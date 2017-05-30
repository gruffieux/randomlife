#include "clookedpickup.h"
#include "ccharinfo.h"
#include "clookedspell.h"
#include "cinventory.h"
#include "cchemicallab.h"
#include "../RLRes/RLRes/resource.h"

extern Display *Screen;
extern MsgReporter EventsReporter;
extern MiniEvents ActionReporter;
extern List PickupPack, ZonePickupPack, SoundPack;
extern CharInfo *PCharInfo;
extern LookedSpell *PMagicSpell;
extern Inventory *PInventory, *PTargetInventory;
extern ChemicalLab *PChemicalLab;

LookedPickup::LookedPickup()
{
	Focus = NULL;
	Observer = NULL;
}

LookedPickup::LookedPickup(int Panel_ColSpace, int Panel_RowSpace, int Panel_BorderSpace) :
Panel(4, 7, Panel_ColSpace, Panel_RowSpace, Panel_BorderSpace, true, Text(), Str()),
Item(NULL, NULL, NULL, NULL, "OBJET")
{
	Focus = NULL;
	Observer = NULL;
}

void LookedPickup::Init()
{
	Grid[0][0].ColSpan = -1;
	Grid[0][0].halign = HALIGN_CENTER;
	Grid[6][0].data = VISUAL;
	Grid[6][1].data = VISUAL;

	AlignCols();

	Panel::Init();
	
	AddForbidden1(GetVisualData(0, 6));
	AddForbidden1(GetVisualData(1, 6));
}

void LookedPickup::initAni()
{
	GetVisualData(0, 6)->addAni(IDB_ACTION_BUTTONS, "", 16, 60, 16, -1, 90);
	GetVisualData(1, 6)->addAni(IDB_ACTION_BUTTONS, "", 16, 60, 16, -1, 90);

	InitCloseButton(IDB_PANEL_CLOSE, "", 16, 16);
}

void LookedPickup::restoreAni()
{
	Panel::restoreAni();

	GetVisualData(0, 6)->restoreAni();
	GetVisualData(1, 6)->restoreAni();
}

void LookedPickup::destroyAni()
{
	Panel::destroyAni();

	GetVisualData(0, 6)->destroyAni();
	GetVisualData(1, 6)->destroyAni();
}

bool LookedPickup::Refresh()
{
	Weapon *pWeapon;
	Armor *pArmor;
	Potion *pPotion;
	Ingredient *pIngredient;
	
	if ((Focus == NULL) | (Observer == NULL))
		return false;

	pWeapon = dynamic_cast<Weapon*>(Focus);
	pArmor = dynamic_cast<Armor*>(Focus);
	pIngredient = dynamic_cast<Ingredient*>(Focus);
	pPotion = dynamic_cast<Potion*>(Focus);
	
	// Cellules à afficher par défaut
	Grid[1][2].display = true;
	Grid[2][2].display = false;
	Grid[3][2].display = false;
	Grid[4][2].display = false;
	Grid[5][2].display = false;
	Grid[6][2].display = false;
	Grid[1][3].display = true;
	Grid[2][3].display = false;
	Grid[3][3].display = false;
	Grid[4][3].display = false;
	Grid[5][3].display = false;
	
	// Affichage du texte par défaut
	Screen->LoadText(((Text*)Grid[0][0].Content)->GetSprite(), NULL, name, RGB(255, 255, 255), RGB(0, 0, 0));
	((Text*)Grid[0][0].Content)->GetSprite()->SetColorKey(RGB(255, 255, 255));
	Screen->LoadText(((Text*)Grid[1][0].Content)->GetSprite(), NULL, "Nom:", RGB(255, 255, 255), RGB(0, 0, 0));
	((Text*)Grid[1][0].Content)->GetSprite()->SetColorKey(RGB(255, 255, 255));
	Screen->LoadText(((Text*)Grid[2][0].Content)->GetSprite(), NULL, "Prix:", RGB(255, 255, 255), RGB(0, 0, 0));
	((Text*)Grid[2][0].Content)->GetSprite()->SetColorKey(RGB(255, 255, 255));
	Screen->LoadText(((Text*)Grid[3][0].Content)->GetSprite(), NULL, "Poids:", RGB(255, 255, 255), RGB(0, 0, 0));
	((Text*)Grid[3][0].Content)->GetSprite()->SetColorKey(RGB(255, 255, 255));
	Screen->LoadText(((Text*)Grid[4][0].Content)->GetSprite(), NULL, "Malus:", RGB(255, 255, 255), RGB(0, 0, 0));
	((Text*)Grid[4][0].Content)->GetSprite()->SetColorKey(RGB(255, 255, 255));
	Screen->LoadText(((Text*)Grid[5][0].Content)->GetSprite(), NULL, "Enchantement:", RGB(255, 255, 255), RGB(0, 0, 0));
	((Text*)Grid[5][0].Content)->GetSprite()->SetColorKey(RGB(255, 255, 255));
	Screen->LoadText(((Text*)Grid[1][1].Content)->GetSprite(), NULL, Focus->GetDisplayedName()->Get(), RGB(255, 255, 255), RGB(0, 0, 0));
	((Text*)Grid[1][1].Content)->GetSprite()->SetColorKey(RGB(255, 255, 255));
	Screen->LoadText(((Text*)Grid[2][1].Content)->GetSprite(), NULL, Focus->GetPrice(), RGB(255, 255, 255), RGB(0, 0, 0));
	((Text*)Grid[2][1].Content)->GetSprite()->SetColorKey(RGB(255, 255, 255));
	Screen->LoadText(((Text*)Grid[3][1].Content)->GetSprite(), NULL, (int)Focus->GetWeight(), RGB(255, 255, 255), RGB(0, 0, 0));
	((Text*)Grid[3][1].Content)->GetSprite()->SetColorKey(RGB(255, 255, 255));
	Screen->LoadText(((Text*)Grid[4][1].Content)->GetSprite(), NULL, BuildString("%s %d, %s %d, %s %d", Observer->GetStatPack()->GetStat(AGILITY)->GetLabel().Get(), Focus->GetMalus()->AGModifier, Observer->GetStatPack()->GetStat(STRENGTH)->GetLabel().Get(), Focus->GetMalus()->STModifier, Observer->GetStatPack()->GetStat(WILLPOWER)->GetLabel().Get(), Focus->GetMalus()->WPModifier), RGB(255, 255, 255), RGB(0, 0, 0));
	((Text*)Grid[4][1].Content)->GetSprite()->SetColorKey(RGB(255, 255, 255));
	Screen->LoadText(((Text*)Grid[5][1].Content)->GetSprite(), NULL, Focus->GetEnchant()->GetDescription(), RGB(255, 255, 255), RGB(0, 0, 0));
	((Text*)Grid[5][1].Content)->GetSprite()->SetColorKey(RGB(255, 255, 255));
	Screen->LoadText(((Text*)Grid[1][2].Content)->GetSprite(), NULL, "Type:", RGB(255, 255, 255), RGB(0, 0, 0));
	((Text*)Grid[1][2].Content)->GetSprite()->SetColorKey(RGB(255, 255, 255));
	Screen->LoadText(((Text*)Grid[1][3].Content)->GetSprite(), NULL, Focus->GetTypeDescription(), RGB(255, 255, 255), RGB(0, 0, 0));
	((Text*)Grid[1][3].Content)->GetSprite()->SetColorKey(RGB(255, 255, 255));
	
	// Affichage pour une arme
	if (pWeapon)
	{
		Grid[1][2].display = true;
		Grid[2][2].display = true;
		Grid[3][2].display = true;
		Grid[4][2].display = false;
		Grid[5][2].display = false;
		Grid[6][2].display = false;
		Grid[1][3].display = true;
		Grid[2][3].display = true;
		Grid[3][3].display = true;
		Grid[4][3].display = false;
		Grid[5][3].display = false;
		Screen->LoadText(((Text*)Grid[2][2].Content)->GetSprite(), NULL, "Dégats min:", RGB(255, 255, 255), RGB(0, 0, 0));
		((Text*)Grid[2][2].Content)->GetSprite()->SetColorKey(RGB(255, 255, 255));
		Screen->LoadText(((Text*)Grid[3][2].Content)->GetSprite(), NULL, "Dégats max:", RGB(255, 255, 255), RGB(0, 0, 0));
		((Text*)Grid[3][2].Content)->GetSprite()->SetColorKey(RGB(255, 255, 255));
		Screen->LoadText(((Text*)Grid[2][3].Content)->GetSprite(), NULL, pWeapon->GetMinDamage(), RGB(255, 255, 255), RGB(0, 0, 0));
		((Text*)Grid[2][3].Content)->GetSprite()->SetColorKey(RGB(255, 255, 255));
		Screen->LoadText(((Text*)Grid[3][3].Content)->GetSprite(), NULL, pWeapon->GetMaxDamage(), RGB(255, 255, 255), RGB(0, 0, 0));
		((Text*)Grid[3][3].Content)->GetSprite()->SetColorKey(RGB(255, 255, 255));
	}
	
	// Affichage pour une armure
	if (pArmor)
	{
		pArmor->GenerateSuccessRate(Observer->GetSkillPack()->GetSkill(pArmor->GetArmorSkill())->GetLevel());
		Grid[1][2].display = true;
		Grid[2][2].display = true;
		Grid[3][2].display = true;
		Grid[4][2].display = false;
		Grid[5][2].display = false;
		Grid[6][2].display = false;
		Grid[1][3].display = true;
		Grid[2][3].display = true;
		Grid[3][3].display = true;
		Grid[4][3].display = false;
		Grid[5][3].display = false;
		
		Screen->LoadText(((Text*)Grid[2][2].Content)->GetSprite(), NULL, "Valeure d'armure:", RGB(255, 255, 255), RGB(0, 0, 0));
		((Text*)Grid[2][2].Content)->GetSprite()->SetColorKey(RGB(255, 255, 255));
		Screen->LoadText(((Text*)Grid[3][2].Content)->GetSprite(), NULL, "Efficacité (perso):", RGB(255, 255, 255), RGB(0, 0, 0));
		((Text*)Grid[3][2].Content)->GetSprite()->SetColorKey(RGB(255, 255, 255));
		Screen->LoadText(((Text*)Grid[2][3].Content)->GetSprite(), NULL, pArmor->GetRating(), RGB(255, 255, 255), RGB(0, 0, 0));
		((Text*)Grid[2][3].Content)->GetSprite()->SetColorKey(RGB(255, 255, 255));
		Screen->LoadText(((Text*)Grid[3][3].Content)->GetSprite(), NULL, pArmor->GetSuccessRate() + (Str)" %", RGB(255, 255, 255), RGB(0, 0, 0));
		((Text*)Grid[3][3].Content)->GetSprite()->SetColorKey(RGB(255, 255, 255));
	}
	
	// Affichage pour une potion
	if (pPotion)
	{
		Grid[1][2].display = true;
		Grid[2][2].display = true;
		Grid[3][2].display = true;
		Grid[4][2].display = false;
		Grid[5][2].display = false;
		Grid[6][2].display = false;
		Grid[1][3].display = true;
		Grid[2][3].display = true;
		Grid[3][3].display = true;
		Grid[4][3].display = false;
		Grid[5][3].display = false;
		
		Screen->LoadText(((Text*)Grid[2][2].Content)->GetSprite(), NULL, "Effet:", RGB(255, 255, 255), RGB(0, 0, 0));
		((Text*)Grid[2][2].Content)->GetSprite()->SetColorKey(RGB(255, 255, 255));
		Screen->LoadText(((Text*)Grid[3][2].Content)->GetSprite(), NULL, "Puissance:", RGB(255, 255, 255), RGB(0, 0, 0));
		((Text*)Grid[3][2].Content)->GetSprite()->SetColorKey(RGB(255, 255, 255));
		Screen->LoadText(((Text*)Grid[2][3].Content)->GetSprite(), NULL, pPotion->GetEffectDescription(), RGB(255, 255, 255), RGB(0, 0, 0));
		((Text*)Grid[2][3].Content)->GetSprite()->SetColorKey(RGB(255, 255, 255));
		Screen->LoadText(((Text*)Grid[3][3].Content)->GetSprite(), NULL, (int)pPotion->GetPower(), RGB(255, 255, 255), RGB(0, 0, 0));
		((Text*)Grid[3][3].Content)->GetSprite()->SetColorKey(RGB(255, 255, 255));
	}
	
	// Frames
	if (Focus->GetStatus()->picked)
	{
		if (Focus->GetOwnerID() == Observer->getId())
		{
			if (Observer->GetStatus()->trading)
			{
				Grid[6][0].PicFrame = 11;
				Grid[6][1].display = false;
			}
			else
			{
				if (Focus->GetGroup() == INGREDIENTS)
					Grid[6][0].PicFrame = 15;
				else
					if (Focus->GetType() == POTION)
						Grid[6][0].PicFrame = 14;
					else
						if (Focus->GetStatus()->equiped)
							Grid[6][0].PicFrame = 2;
						else
							Grid[6][0].PicFrame = 1;
				Grid[6][1].PicFrame = 3;
				Grid[6][1].display = true;
			}
		}
		else
		{
			if (Observer->GetTarget()->GetStatus()->dead)
			{
				Grid[6][0].PicFrame = 0;
				Grid[6][1].display = false;
			}
			else
				if (Observer->GetStatus()->trading)
				{
					Grid[6][0].PicFrame = 10;
					Grid[6][1].display = false;
				}
				else
				{
					Grid[6][0].PicFrame = 5;
					Grid[6][1].display = false;
				}
		}
	}
	else
	{
		Grid[6][0].PicFrame = 0;
		Grid[6][1].display = false;
	}

	return Panel::Refresh();
}

bool LookedPickup::Run(Item *Cursor)
{
	if (Focus) return Panel::Run(Cursor);

	return false;
}

bool LookedPickup::generate(bool waited, Cursor *cursor)
{
	Potion *pPotion;
	Ingredient *pIngredient;
	
	if ((Focus == NULL) | (Observer == NULL))
		return false;

	pIngredient = dynamic_cast<Ingredient*>(Focus);
	pPotion = dynamic_cast<Potion*>(Focus);

	// Evénements sur bouton gauche
	if (ClickOnGrid(0, 6, waited, cursor))
	{
		if (Focus->GetStatus()->picked)
		{
			if (Focus->GetOwnerID() == Observer->getId())
			{
				if (Observer->GetStatus()->trading)
				{
					if (Observer->GetHumanizedTarget()->Buy(Focus, Observer, &PickupPack, &EventsReporter, &ActionReporter) == ACTION_SUCCESS)
					{
						Sound::getSoundElement(TRADING, &SoundPack)->play(DMUS_SEGF_SECONDARY);
						PInventory->Refresh();
						PTargetInventory->Refresh();
					}
				}
				else
					if (pIngredient)
					{
						if (!Observer->GetStatus()->fight)
						{
							if (!Observer->GetCrafting())
							{
								Potion *pPotion = new Potion(NULL, NONE_PICKUP_POTION_EFFECT, 20, 1, NONE_PICKUP_UNIQUE_ID, NONE_CHAR_CLASS, 6, 15, BuildString("Potion %d", Observer->GetCraftPack()->GetElementCount()));
								Observer->SetCrafting(pPotion);
								Observer->GetCrafting()->initAni();
								PickupPack.AddElement(pPotion);
							}
							if (Observer->GetCrafting())
								switch (Focus->GetType())
								{
								case FLOWER:
									Observer->AddPotionElement(0, pIngredient);
									break;
								case FRUIT:
									Observer->AddPotionElement(1, pIngredient);
									break;
								case DRINK:
									Observer->AddPotionElement(2, pIngredient);
									break;
								}
							PChemicalLab->show();
							PChemicalLab->Refresh();
						}
					}
					else
						if (pPotion)
						{
							if (Observer->Consume(pPotion, &PickupPack, &EventsReporter))
							{
								Sound::getSoundElement(POTION_DRINK, &SoundPack)->play(DMUS_SEGF_SECONDARY);
								PInventory->Refresh();
							}
						}
						else
							if (Focus->GetStatus()->equiped)
							{
								if (Observer->UnEquip(Focus, false, &EventsReporter) == PICKUP_SUMMONED)
									PInventory->Refresh();
								PCharInfo->GetPStats()->Refresh();
								PCharInfo->GetPStats2()->Refresh();
								PCharInfo->GetPMagicSchools()->Refresh();
								PMagicSpell->Refresh();
							}
							else
							{
								if (Observer->Equip(Focus, &EventsReporter) == PICKUP_SUMMONED)
									PInventory->Refresh();
								PCharInfo->GetPStats()->Refresh();
								PCharInfo->GetPStats2()->Refresh();
								PCharInfo->GetPMagicSchools()->Refresh();
								PMagicSpell->Refresh();
							}
			}
			else
				if (Observer->GetTarget()->GetStatus()->dead)
				{
					if (Observer->GetHumanizedTarget()->Give(Focus, Observer, &EventsReporter, &ActionReporter) == ACTION_SUCCESS)
					{
						PInventory->Refresh();
						PTargetInventory->Refresh();
					}
				}
				else
					if (Observer->GetStatus()->trading)
					{
						if (Observer->Buy(Focus, Observer->GetHumanizedTarget(), &PickupPack, &EventsReporter, &ActionReporter) == ACTION_SUCCESS)
						{
							Sound::getSoundElement(TRADING, &SoundPack)->play(DMUS_SEGF_SECONDARY);
							PInventory->Refresh();
							PTargetInventory->Refresh();
						}
					}
					else
						if (Observer->GetAdjacent(Observer->GetTarget()))
							Observer->Steal(Focus);
						else
						{
							Observer->PlaySound(Sound::getSoundElement(ACTION_FORBIDDEN, &SoundPack), 0, false);
							EventsReporter.AddMessage(ACTION_DENIED, 4);
						}
		}
		else
			if (Observer->AddToEquipment(Focus, &EventsReporter, &ActionReporter) == ACTION_SUCCESS)
			{
				ZonePickupPack.RemoveElement(Focus);
				PInventory->Refresh();
			}

		hide();
		Focus = NULL;
	}

	// Evénements sur bouton droite
	if (ClickOnGrid(1, 6, waited, cursor))
	{
		if (Focus->GetStatus()->picked)
		{
			if (Observer->Drop(Focus, &EventsReporter) == ACTION_SUCCESS)
				ZonePickupPack.AddElement(Focus);
			hide();
			Focus = NULL;
		}
		PCharInfo->GetPStats()->Refresh();
		PInventory->Refresh();
		PMagicSpell->Refresh();
	}

	return Panel::generate(waited, cursor);
}
