#include "cdataprogress.h"
#include "main.h"
#include "../RLRes/RLRes/resource.h"

extern Display *Screen;
extern DWORD WINAPI LoadData(LPVOID lpParam);
extern DWORD WINAPI SaveData(LPVOID lpParam);

DataProgress::DataProgress()
{
	Thread = NULL;
	DataBar = new EnergyBar();
}

DataProgress::DataProgress(int Panel_ColSpace, int Panel_RowSpace, int Panel_BorderSpace) :
Panel(1, 2, Panel_ColSpace, Panel_RowSpace, Panel_BorderSpace, false, Text(), NULL, NULL, Str())
{
	Thread = NULL;
	DataBar = new EnergyBar(0, NULL, NULL, 1, XP, NULL, NULL, 0, 16);
}

void DataProgress::Init()
{
	Grid[1][0].data = VISUAL_INHERIT;

	Panel::Init();

	Grid[1][0].Content = DataBar;
}

void DataProgress::initAni()
{
	InitBackground(IDB_PANEL_BKG, "", 16, 16);
	initBorders(IDB_PANEL_BORDER, "", 61, 6);
	DataBar->addAni(IDB_ENERGYBAR, "", 5, 1, 16, -1, 0);
	DataBar->GetBar()->addAni(IDB_ENERGYBAR, "", 5, 1, 16, -1, 0);
}

void DataProgress::restoreAni()
{
	Panel::restoreAni();
	DataBar->restoreAni();
	DataBar->GetBar()->restoreAni();
}

void DataProgress::destroyAni()
{
	Panel::destroyAni();
	DataBar->destroyAni();
	DataBar->GetBar()->destroyAni();
}

bool DataProgress::Refresh()
{
	if (name.GetLength())
	{
		Screen->LoadText(GetTextData(0, 0)->GetSprite(), NULL, name, RGB(255, 255, 255), RGB(0, 0, 0));
		GetTextData(0, 0)->GetSprite()->SetColorKey(RGB(255, 255, 255));
	}

	return Panel::Refresh();
}

bool DataProgress::CreateLoadThread()
{
	if (Thread) return false;

	Thread = CreateThread(NULL, 0, LoadData, NULL, NULL, NULL);

	return true;
}

bool DataProgress::CreateSaveThread()
{
	if (Thread) return false;

	Thread = CreateThread(NULL, 0, SaveData, NULL, NULL, NULL);

	return true;
}

void DataProgress::StopThread()
{
	CloseHandle(Thread);
	Thread = NULL;
}
