#include "cgauge.h"
#include "chumanized.h"
#include "../RLRes/RLRes/resource.h"

extern Display *Screen;

Gauge::Gauge()
{
	PActions = new Panel();
	Focus = NULL;
}

Gauge::Gauge(int Panel_ColSpace, int Panel_RowSpace, int Panel_BorderSpace) :
Panel(2, 6, Panel_ColSpace, Panel_RowSpace, Panel_BorderSpace, true, Text(), Str()),
Item(NULL, NULL, NULL, NULL, "JAUGE")
{
	PActions = new Panel(TOTAL_COMBAT_PHASE, 1, 0, 0, 0, false, Text(), Str());
	Focus = NULL;
}

void Gauge::SetFocus(Character *Gauge_Focus)
{
	Humanized *pHumanized;

	Focus = Gauge_Focus;
	pHumanized = dynamic_cast<Humanized*>(Focus);

	Grid[3][1].Content = Focus->GetHitPoints();

	if (pHumanized)
		Grid[4][1].Content = pHumanized->GetManaPoints();
	else
		Grid[4][1].Content = NULL;
}

void Gauge::Init()
{
	int i;

	for (i = 1; i < RowCount; i++)
		AlignColsOnRow(i);
	AlignRows();

	Grid[0][0].ColSpan = -1;
	Grid[0][0].halign = HALIGN_CENTER;
	Grid[3][1].data = VISUAL_INHERIT;
	Grid[4][1].data = VISUAL_INHERIT;
	Grid[5][1].data = VISUAL_INHERIT;
	
	for (i = 0; i < TOTAL_COMBAT_PHASE; i++)
		PActions->GetGrid(i, 0)->data = VISUAL;

	Panel::Init();
	PActions->Init();

	Grid[5][1].Content = PActions;
}

void Gauge::Initialize()
{
	Panel::Initialize();
	PActions->Initialize();
}

void Gauge::initAni()
{
	int i;
	
	for (i = 0; i < TOTAL_COMBAT_PHASE; i++)
		PActions->GetVisualData(i, 0)->addAni(IDB_COMBAT_ACTION, "", 2, 16, 16, -1, 90);
}

void Gauge::restoreAni()
{
	int i;
	
	Panel::restoreAni();
	
	for (i = 0; i < TOTAL_COMBAT_PHASE; i++)
		PActions->GetVisualData(i, 0)->restoreAni();
}

void Gauge::destroyAni()
{
	int i;
	
	Panel::destroyAni();
	
	for (i = 0; i < TOTAL_COMBAT_PHASE; i++)
		PActions->GetVisualData(i, 0)->destroyAni();
}

void Gauge::DestroyText()
{
	Panel::DestroyText();
	PActions->DestroyText();
}

bool Gauge::Run(Item *Cursor)
{
	if (Focus) return Panel::Run(Cursor);

	return false;
}

bool Gauge::Refresh()
{
	int i;

	if (!Focus) return false;

	for (i = 0; i < TOTAL_COMBAT_PHASE; i++)
		if (Focus->GetCombat()->action[i])
			PActions->GetGrid(i, 0)->PicFrame = 0;
		else
			PActions->GetGrid(i, 0)->PicFrame = 1;

	PActions->Refresh();

	Screen->LoadText(((Text*)Grid[0][0].Content)->GetSprite(), NULL, name, RGB(255, 255, 255), RGB(0, 0, 0));
	((Text*)Grid[0][0].Content)->GetSprite()->SetColorKey(RGB(255, 255, 255));
	Screen->LoadText(((Text*)Grid[1][0].Content)->GetSprite(), NULL, "Nom:", RGB(255, 255, 255), RGB(0, 0, 0));
	((Text*)Grid[1][0].Content)->GetSprite()->SetColorKey(RGB(255, 255, 255));
	((Text*)Grid[1][1].Content)->Update(112, RGB(192, 192, 192), RGB(0, 0, 0), Focus->GetDisplayedName());
	Screen->LoadText(((Text*)Grid[2][0].Content)->GetSprite(), NULL, "Titre:", RGB(255, 255, 255), RGB(0, 0, 0));
	((Text*)Grid[2][0].Content)->GetSprite()->SetColorKey(RGB(255, 255, 255));
	((Text*)Grid[2][1].Content)->Update(112, RGB(192, 192, 192), RGB(0, 0, 0), Focus->GetTitle());
	Screen->LoadText(((Text*)Grid[3][0].Content)->GetSprite(), NULL, "Vie:", RGB(255, 255, 255), RGB(0, 0, 0));
	((Text*)Grid[3][0].Content)->GetSprite()->SetColorKey(RGB(255, 255, 255));
	Screen->LoadText(((Text*)Grid[4][0].Content)->GetSprite(), NULL, "Mana:", RGB(255, 255, 255), RGB(0, 0, 0));
	((Text*)Grid[4][0].Content)->GetSprite()->SetColorKey(RGB(255, 255, 255));
	Screen->LoadText(((Text*)Grid[5][0].Content)->GetSprite(), NULL, "Actions:", RGB(255, 255, 255), RGB(0, 0, 0));
	((Text*)Grid[5][0].Content)->GetSprite()->SetColorKey(RGB(255, 255, 255));

	return Panel::Refresh();
}

bool Gauge::generate(bool waited, Cursor *cursor)
{
	PActions->generate(waited, cursor);
	
	return Panel::generate(waited, cursor);
}
