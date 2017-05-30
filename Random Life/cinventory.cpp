#include "cinventory.h"
#include "../RLRes/RLRes/resource.h"

extern Display *Screen;

Inventory::Inventory()
{
	Focus = NULL;
	Others = new Pocket();
	Weapons = new Pocket();
	Armors = new Pocket();
	Ingredients = new Pocket();
}

Inventory::Inventory(int Panel_ColSpace, int Panel_RowSpace, int Panel_BorderSpace, bool Panel_save, Str Object_Name) :
Panel(4, 4, Panel_ColSpace, Panel_RowSpace, Panel_BorderSpace, Panel_save, Text(), Str()),
Item(NULL, NULL, NULL, NULL, Object_Name)
{
	Focus = NULL;
	Others = new Pocket(2, 2, 5, 64, 192, "Divers");
	Weapons = new Pocket(2, 2, 5, 64, 192, "Armes");
	Armors = new Pocket(2, 2, 5, 64, 192, "Armures");
	Ingredients = new Pocket(2, 2, 5, 80, 192, "Ingrédients");
}

Inventory::~Inventory()
{
	delete Others;
	delete Weapons;
	delete Armors;
	delete Ingredients;
}

void Inventory::SetFocus(Humanized *Inventory_Focus)
{
	Focus = Inventory_Focus;

	if (Focus)
	{
		Grid[2][1].Content = Focus->GetChargePoints();
		Others->SetFocus(Focus->GetPocket(OTHERS));
		Weapons->SetFocus(Focus->GetPocket(WEAPONS));
		Armors->SetFocus(Focus->GetPocket(ARMORS));
		Ingredients->SetFocus(Focus->GetPocket(INGREDIENTS));
	}
}

void Inventory::Init()
{
	int x;

	Grid[0][0].ColSpan = -1;
	Grid[0][0].halign = HALIGN_CENTER;
	Grid[2][1].data = VISUAL_INHERIT;
	for (x = 0; x < ColCount; x++)
		Grid[3][x].data = VISUAL_INHERIT;

	Panel::Init();
	Others->Init();
	Weapons->Init();
	Armors->Init();
	Ingredients->Init();

	Grid[3][0].Content = Others;
	Grid[3][1].Content = Weapons;
	Grid[3][2].Content = Armors;
	Grid[3][3].Content = Ingredients;
}

void Inventory::Initialize()
{
	Others->Initialize();
	Weapons->Initialize();
	Armors->Initialize();
	Ingredients->Initialize();
	Panel::Initialize();
}

void Inventory::initAni()
{
	Others->initAni();
	Others->InitIcons(OTHERS);

	Weapons->initAni();
	Weapons->InitIcons(WEAPONS);

	Armors->initAni();
	Armors->InitIcons(ARMORS);

	Ingredients->initAni();
	Ingredients->InitIcons(INGREDIENTS);

	InitCloseButton(IDB_PANEL_CLOSE, "", 16, 16);
}

void Inventory::restoreAni()
{
	Panel::restoreAni();
	Others->restoreAni();
	Weapons->restoreAni();
	Armors->restoreAni();
	Ingredients->restoreAni();
}

void Inventory::destroyAni()
{
	Panel::destroyAni();
	Others->destroyAni();
	Weapons->destroyAni();
	Armors->destroyAni();
	Ingredients->destroyAni();
}

void Inventory::DestroyText()
{
	Panel::DestroyText();
	Others->DestroyText();
	Weapons->DestroyText();
	Armors->DestroyText();
	Ingredients->DestroyText();
}

void Inventory::AddForbidden2(Item *pItem)
{
	Panel::AddForbidden2(pItem);
	Others->AddForbidden2(pItem);
	Others->AddForbidden2(pItem);
	Armors->AddForbidden2(pItem);
	Ingredients->AddForbidden2(pItem);
}

void Inventory::RemoveForbidden2(Item *pItem)
{
	Panel::RemoveForbidden2(pItem);
	Others->RemoveForbidden2(pItem);
	Others->RemoveForbidden2(pItem);
	Armors->RemoveForbidden2(pItem);
	Ingredients->RemoveForbidden2(pItem);
}

bool Inventory::Run(Item *Cursor)
{
	if (Focus) return Panel::Run(Cursor);

	return false;
}

bool Inventory::Refresh()
{
	Others->Refresh();
	Weapons->Refresh();
	Armors->Refresh();
	Ingredients->Refresh();

	if (!Focus) return false;

	Screen->LoadText(((Text*)Grid[0][0].Content)->GetSprite(), NULL, name, RGB(255, 255, 255), RGB(0, 0, 0));
	((Text*)Grid[0][0].Content)->GetSprite()->SetColorKey(RGB(255, 255, 255));
	Screen->LoadText(((Text*)Grid[1][0].Content)->GetSprite(), NULL, "Bourse:", RGB(255, 255, 255), RGB(0, 0, 0));
	((Text*)Grid[1][0].Content)->GetSprite()->SetColorKey(RGB(255, 255, 255));
	Screen->LoadText(((Text*)Grid[1][1].Content)->GetSprite(), NULL, Focus->GetPurse(), RGB(255, 255, 255), RGB(0, 0, 0));
	((Text*)Grid[1][1].Content)->GetSprite()->SetColorKey(RGB(255, 255, 255));
	Screen->LoadText(((Text*)Grid[2][0].Content)->GetSprite(), NULL, "Charge:", RGB(255, 255, 255), RGB(0, 0, 0));
	((Text*)Grid[2][0].Content)->GetSprite()->SetColorKey(RGB(255, 255, 255));

	return Panel::Refresh();
}

bool Inventory::generate(bool waited, Cursor *cursor)
{
	Others->generate(waited, cursor);
	Weapons->generate(waited, cursor);
	Armors->generate(waited, cursor);
	Ingredients->generate(waited, cursor);

	if (Others->GetSelector()->Slide(cursor))
		Others->Refresh();
	if (Weapons->GetSelector()->Slide(cursor))
		Weapons->Refresh();
	if (Armors->GetSelector()->Slide(cursor))
		Armors->Refresh();
	if (Ingredients->GetSelector()->Slide(cursor))
		Ingredients->Refresh();

	return Panel::generate(waited, cursor);
}

bool Inventory::CanMove(Item *pItem)
{
	if (!Others->CanMove(pItem)) return false;
	if (!Weapons->CanMove(pItem)) return false;
	if (!Armors->CanMove(pItem)) return false;
	if (!Ingredients->CanMove(pItem)) return false;

	return Panel::CanMove(pItem);
}

bool Inventory::CanClick(Item *pItem)
{
	if (!Others->CanClick(pItem)) return false;
	if (!Weapons->CanClick(pItem)) return false;
	if (!Armors->CanClick(pItem)) return false;
	if (!Ingredients->CanClick(pItem)) return false;

	return Panel::CanClick(pItem);
}
