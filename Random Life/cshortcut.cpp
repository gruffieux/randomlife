#include "cshortcut.h"
#include "main.h"
#include "../RLRes/RLRes/resource.h"

extern Display *Screen;
extern void DisplayPanel(SHORT_PANEL ShortKey);

ShortCut::ShortCut(int Panel_ColSpace, int Panel_RowSpace, int Panel_BorderSpace) :
IconList(false, 8, Panel_ColSpace, Panel_RowSpace, Panel_BorderSpace, true, Str()),
Item(NULL, NULL, NULL, NULL, "RACC")
{
}

void ShortCut::initAni()
{
	IconList::initAni();

	InitIcons(IDB_SHORTCUT_LOGOS, "", 7, 32, 32);
	InitCloseButton(IDB_PANEL_CLOSE, "", 16, 16);
}

void ShortCut::Init()
{
	int i;

	for (i = 1; i < RowCount; i++)
		Grid[i][1].PicFrame = i-1;

	IconList::Init();
}

bool ShortCut::generate(bool waited, Cursor *cursor)
{
	int i;

	for (i = 1; i < RowCount; i++)
		if (ClickOnGrid(1, i, waited, cursor))
			DisplayPanel((SHORT_PANEL)(i-1));

	return IconList::generate(waited, cursor);
}
