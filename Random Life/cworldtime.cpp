#include "cworldtime.h"
#include "main.h"
#include "../RLRes/RLRes/resource.h"

extern Display *Screen;

WorldTime::WorldTime()
{
	day = 0;
	MeditateProgress = 1;
	chao = 0;
	NewDay = false;
	TodayBar = new EnergyBar();
	ChaoBar = new EnergyBar();
}

WorldTime::WorldTime(int Panel_ColSpace, int Panel_RowSpace, int Panel_BorderSpace) :
Panel(2, 2, Panel_ColSpace, Panel_RowSpace, Panel_BorderSpace, true, Text(), Str()),
Item(NULL, NULL, NULL, NULL, "WORLDTIME")
{
	day = 0;
	MeditateProgress = 1;
	chao = 0;
	NewDay = false;
	TodayBar = new EnergyBar(0, WORLD_TIME_SEC_PER_DAY, 60, 1, XP, NULL, NULL, 0, 16);
	ChaoBar = new EnergyBar(0, WORLD_TIME_SEC_BEFORE_CHAO, 60, 12, LIFE, NULL, NULL, 0, 16);
}

WorldTime::~WorldTime()
{
	delete TodayBar;
	delete ChaoBar;
}

void WorldTime::Reset()
{
	day = 0;
	MeditateProgress = 1;
	chao = 0;
	NewDay = false;
	TodayBar->SetAmount(0);
	ChaoBar->SetAmount(0);
}

void WorldTime::Init()
{
	AlignCols();
	Grid[0][1].data = VISUAL_INHERIT;
	Grid[1][1].data = VISUAL_INHERIT;

	Panel::Init();
	
	ChaoBar->SetAmount(0);

	Grid[0][1].Content = TodayBar;
	Grid[1][1].Content = ChaoBar;
}

void WorldTime::initAni()
{
	TodayBar->addAni(IDB_ENERGYBAR, "", 5, 1, 16, -1, 0);
	TodayBar->GetBar()->addAni(IDB_ENERGYBAR, "", 5, 1, 16, -1, 0);
	ChaoBar->addAni(IDB_ENERGYBAR, "", 5, 1, 16, -1, 0);
	ChaoBar->GetBar()->addAni(IDB_ENERGYBAR, "", 5, 1, 16, -1, 0);
}

void WorldTime::restoreAni()
{
	Panel::restoreAni();
	
	TodayBar->restoreAni();
	ChaoBar->restoreAni();
}

void WorldTime::destroyAni()
{
	Panel::destroyAni();
	
	TodayBar->destroyAni();
	ChaoBar->destroyAni();
}

void WorldTime::GoNewDay()
{
	day++;
	MeditateProgress = 1;
	NewDay = false;
	TodayBar->SetAmount(0);
}

bool WorldTime::Refresh()
{
	Screen->LoadText(((Text*)Grid[0][0].Content)->GetSprite(), NULL, BuildString("Jour %d:", day), RGB(255, 255, 255), RGB(0, 0, 0));
	((Text*)Grid[0][0].Content)->GetSprite()->SetColorKey(RGB(255, 255, 255));
	Screen->LoadText(((Text*)Grid[1][0].Content)->GetSprite(), NULL, "Chao:", RGB(255, 255, 255), RGB(0, 0, 0));
	((Text*)Grid[1][0].Content)->GetSprite()->SetColorKey(RGB(255, 255, 255));

	return Panel::Refresh();
}

bool WorldTime::generate(bool waited, Cursor *cursor)
{
	ChaoBar->GenerateBar();

	return Panel::generate(waited, cursor);
}
