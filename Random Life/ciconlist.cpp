#include "ciconlist.h"
#include "../RLRes/RLRes/resource.h"

extern Display *Screen;

IconList::IconList(bool UseSelector, int Panel_RowCount, int Panel_ColSpace, int Panel_RowSpace, int Panel_BorderSpace, bool Panel_save, Str Object_Name) :
Panel(2, Panel_RowCount, Panel_ColSpace, Panel_RowSpace, Panel_BorderSpace, Panel_save, Text(), Str()),
Item(NULL, NULL, NULL, NULL, Object_Name)
{
	if (UseSelector)
		Selector = new Elevator(true, false, false, 16, 16, "Sélecteur d'icône");
	else
		Selector = NULL;
}

IconList::IconList(bool UseSelector, int Panel_RowCount, int Panel_ColSpace, int Panel_RowSpace, int Panel_BorderSpace, bool Panel_save, int Item_width, int Item_height, Str Object_Name) :
Panel(2, Panel_RowCount, Panel_ColSpace, Panel_RowSpace, Panel_BorderSpace, Panel_save, Text(), NULL, NULL, Str()),
Item(NULL, NULL, Item_width, Item_height, Object_Name)
{
	if (UseSelector)
		Selector = new Elevator(true, false, false, 16, 16, "Sélecteur d'icône");
	else
		Selector = NULL;
}

void IconList::Init()
{
	int y;

	Grid[0][0].ColSpan = -1;
	Grid[1][0].data = VISUAL_INHERIT;
	Grid[1][0].mosaic = true;

	for (y = 1; y < RowCount; y++)
	{
		Grid[y][1].data = VISUAL;
		AlignColsOnRow(y);
	}

	Panel::Init();

	if (Selector)
	{
		Grid[1][0].Content = Selector;
		AddForbidden1(Selector);
	}

	for (y = 1; y < RowCount; y++)
		AddForbidden1(GetVisualData(1, y));
}

void IconList::initAni()
{
	if (Selector)
	{
		Selector->addAni(IDB_ELEVATOR_BKG, "", 1, 16, 16, -1, 0);
		Selector->GetCage()->addAni(IDB_ELEVATOR, "", 1, 16, 16, -1, 0);
	}
}

void IconList::InitIcons(int BmpId, Str SkinId, int nFrame, int width, int height)
{
	int y;

	for (y = 1; y < RowCount; y++)
		GetVisualData(1, y)->addAni(BmpId, SkinId, nFrame, width, height, -1, 0);
}

void IconList::Initialize()
{
	Panel::Initialize();

	if (Selector)
	{
		*Selector->getDimension() = Axe(16, (GetVisualData(1, 1)->getDimension()->x * RowCount - GetTextData(0, 0)->GetSprite()->GetHeight()) / 17 * 16);
		*Selector->GetCage()->getDimension() = Axe(16, 16);
	}
}

void IconList::restoreAni()
{
	int y;

	Panel::restoreAni();
	if (Selector) Selector->restoreAni();

	for (y = 1; y < RowCount; y++)
		GetVisualData(1, y)->restoreAni();
}

void IconList::destroyAni()
{
	int y;

	Panel::destroyAni();
	if (Selector) Selector->destroyAni();

	for (y = 1; y < RowCount; y++)
		GetVisualData(1, y)->destroyAni();
}

bool IconList::Refresh()
{
	if (name.GetLength())
	{
		Screen->LoadText(((Text*)Grid[0][0].Content)->GetSprite(), NULL, name, RGB(255, 255, 255), RGB(0, 0, 0));
		((Text*)Grid[0][0].Content)->GetSprite()->SetColorKey(RGB(255, 255, 255));
	}

	return Panel::Refresh();
}
