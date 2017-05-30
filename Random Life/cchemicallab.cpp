#include "cchemicallab.h"
#include "cinventory.h"
#include "../RLRes/RLRes/resource.h"

extern Display *Screen;
extern MsgReporter EventsReporter;
extern Inventory *PInventory;

ChemicalLab::ChemicalLab(int Panel_ColSpace, int Panel_RowSpace, int Panel_BorderSpace) :
Panel(3, 5, Panel_ColSpace, Panel_RowSpace, Panel_BorderSpace, true, Text(), Str()),
Item(NULL, NULL, NULL, NULL, "CHIMIE")
{
	Chemist = NULL;
}

void ChemicalLab::Init()
{
	int i;
	
	Grid[0][0].ColSpan = -1;
	Grid[0][0].halign = HALIGN_CENTER;
	Grid[4][0].data = VISUAL;
	Grid[4][0].PicFrame = 6;

	for (i = 0; i < ColCount; i++)
		Grid[1][i].data = VISUAL;

	Panel::Init();

	AddForbidden1(GetVisualData(0, 4));
}

void ChemicalLab::initAni()
{
	int i;
	
	for (i = 0; i < ColCount; i++)
		GetVisualData(i, 1)->addAni(IDB_POCKET_INGREDIENTS, "", 12, 32, 32, -1, 90);
	
	GetVisualData(0, 4)->addAni(IDB_ACTION_BUTTONS, "", 16, 60, 16, -1, 90);

	InitCloseButton(IDB_PANEL_CLOSE, "", 16, 16);
}

void ChemicalLab::restoreAni()
{
	int i;
	
	Panel::restoreAni();
	
	for (i = 0; i < ColCount; i++)
		GetVisualData(i, 1)->restoreAni();
	
	GetVisualData(0, 4)->restoreAni();
}

void ChemicalLab::destroyAni()
{
	int i;
	
	Panel::destroyAni();
	
	for (i = 0; i < ColCount; i++)
		GetVisualData(i, 1)->destroyAni();
	
	GetVisualData(0, 4)->destroyAni();
}

bool ChemicalLab::Refresh()
{
	if (!Chemist) return false;

	Screen->LoadText(((Text*)Grid[0][0].Content)->GetSprite(), NULL, name, RGB(255, 255, 255), RGB(0, 0, 0));
	((Text*)Grid[0][0].Content)->GetSprite()->SetColorKey(RGB(255, 255, 255));
	Screen->LoadText(((Text*)Grid[2][0].Content)->GetSprite(), NULL, "Effet: ", RGB(255, 255, 255), RGB(0, 0, 0));
	((Text*)Grid[2][0].Content)->GetSprite()->SetColorKey(RGB(255, 255, 255));
	Screen->LoadText(((Text*)Grid[3][0].Content)->GetSprite(), NULL, "Puissance: ", RGB(255, 255, 255), RGB(0, 0, 0));
	((Text*)Grid[3][0].Content)->GetSprite()->SetColorKey(RGB(255, 255, 255));
	
	// Ingrédient fleur
	if (Chemist->getPotionElement(0))
	{
		if (Chemist->GetCrafting())
			Chemist->GetCrafting()->SetElementPower(0, Chemist->getPotionElement(0)->GetEffectFactor(Chemist->GetCrafting()->GetEffect()));
		switch (Chemist->getPotionElement(0)->GetIngredientType())
		{
		case FLOWER_WHITE:
			Grid[1][0].PicFrame = 1;
			break;
		case FLOWER_RED:
			Grid[1][0].PicFrame = 2;
			break;
		case FLOWER_BLUE:
			Grid[1][0].PicFrame = 3;
			break;
		}
	}
	else
		Grid[1][0].PicFrame = 0;
	
	// Ingrédient fruit
	if (Chemist->getPotionElement(1))
	{
		if (Chemist->GetCrafting())
			Chemist->GetCrafting()->SetElementPower(1, Chemist->getPotionElement(1)->GetEffectFactor(Chemist->GetCrafting()->GetEffect()));
		switch (Chemist->getPotionElement(1)->GetIngredientType())
		{
		case FRUIT_RED:
			Grid[1][1].PicFrame = 4;
			break;
		case FRUIT_PINK:
			Grid[1][1].PicFrame = 5;
			break;
		case FRUIT_BLUE:
			Grid[1][1].PicFrame = 6;
			break;
		case FRUIT_BLACK:
			Grid[1][1].PicFrame = 7;
			break;
		case FRUIT_ORANGE:
			Grid[1][1].PicFrame = 8;
			break;
		}
	}
	else
		Grid[1][1].PicFrame = 0;
	
	// Ingrédient liquide
	if (Chemist->getPotionElement(2))
	{
		if (Chemist->GetCrafting())
			Chemist->GetCrafting()->SetElementPower(2, Chemist->getPotionElement(2)->GetEffectFactor(Chemist->GetCrafting()->GetEffect()));
		switch (Chemist->getPotionElement(2)->GetIngredientType())
		{
		case DRINK_WATER:
			Grid[1][2].PicFrame = 9;
			break;
		case DRINK_WINE:
			Grid[1][2].PicFrame = 10;
			break;
		case DRINK_TROLLFLUID:
			Grid[1][2].PicFrame = 11;
			break;
		}
	}
	else
		Grid[1][2].PicFrame = 0;
	
	if (Chemist->GetCrafting())
	{
		if (!Chemist->GetCrafting()->GetElementPower(2))
			Chemist->GetCrafting()->SetElementPower(2, 1);
		Chemist->GetCrafting()->SetPower((float)(Chemist->GetStat2Pack()->GetStat(MAGICAL_AFFINITY)->GetScore() * ((Chemist->GetCrafting()->GetElementPower(0) + Chemist->GetCrafting()->GetElementPower(1)) / Chemist->GetCrafting()->GetElementPower(2))));
		Screen->LoadText(((Text*)Grid[2][1].Content)->GetSprite(), NULL, Chemist->GetCrafting()->GetEffectDescription(), RGB(255, 255, 255), RGB(0, 0, 0));
		((Text*)Grid[2][1].Content)->GetSprite()->SetColorKey(RGB(255, 255, 255));
		Screen->LoadText(((Text*)Grid[3][1].Content)->GetSprite(), NULL, (int)Chemist->GetCrafting()->GetPower(), RGB(255, 255, 255), RGB(0, 0, 0));
		((Text*)Grid[3][1].Content)->GetSprite()->SetColorKey(RGB(255, 255, 255));
	}

	return Panel::Refresh();
}

bool ChemicalLab::generate(bool waited, Cursor *cursor)
{
	if (!Chemist) return false;

	// Evénement sur le bouton valider
	if (ClickOnGrid(0, 4, waited, cursor))
	{
		Chemist->CraftPotion(&EventsReporter);
		WaitForRefresh();
		PInventory->WaitForRefresh();
	}
	
	return Panel::generate(waited, cursor);
}
