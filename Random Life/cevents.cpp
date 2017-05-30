#include <cmsgreporter.h>
#include <cmsg.h>
#include "cevents.h"
#include "main.h"
#include "../RLRes/RLRes/resource.h"

MsgReporter EventsReporter("Main.log", TOTAL_MESSAGE_TYPE);
extern Display *Screen;

Events::Events(int Panel_ColSpace, int Panel_RowSpace, int Panel_BorderSpace, int Item_width, int Item_height) :
Panel(2, 16, Panel_ColSpace, Panel_RowSpace, Panel_BorderSpace, true, Text(), NULL, NULL, Str()),
Item(NULL, NULL, Item_width, Item_height, "EVENEMENTS")
{
	Selector = new Elevator(true, false, false, 16, 16, "Sélecteur d'événement");
}

void Events::Init()
{
	int i;

	Grid[0][0].ColSpan = -1;
	Grid[0][0].halign = HALIGN_CENTER;
	Grid[1][0].data = VISUAL_INHERIT;
	Grid[1][0].mosaic = true;

	for (i = 1; i < RowCount; i++)
		AlignColsOnRow(i);

	Panel::Init();

	Grid[1][0].Content = Selector;
	AddForbidden1(Selector);
	*Selector->getDimension() = Axe(16, 224);
	*Selector->GetCage()->getDimension() = Axe(16, 16);
}

void Events::initAni()
{
	Selector->addAni(IDB_ELEVATOR_BKG, "", 1, 16, 16, -1, 0);
	Selector->GetCage()->addAni(IDB_ELEVATOR, "", 1, 16, 16, -1, 0);
	InitCloseButton(IDB_PANEL_CLOSE, "", 16, 16);
}

void Events::restoreAni()
{
	Panel::restoreAni();
	Selector->restoreAni();
}

void Events::destroyAni()
{
	Panel::destroyAni();
	Selector->destroyAni();
}

bool Events::Refresh()
{
	int i, EventID;

	Screen->LoadText(((Text*)Grid[0][0].Content)->GetSprite(), NULL, name, RGB(255, 255, 255), RGB(0, 0, 0));
	((Text*)Grid[0][0].Content)->GetSprite()->SetColorKey(RGB(255, 255, 255));
	
	// Affichage en défilement des événements
	for (i = 1; i < RowCount - 1; i++)
	{
		EventID = i - 1 + Selector->GetStage(EventsReporter.GetElementCount()-1);
		if ((EventsReporter.GetElementCount() > EventID) && (EventsReporter.GetElementCount() > 0) && (Grid[i][1].posy + Grid[i][1].height <= dimension.y))
		{
			Grid[i][1].display = true;
			((Text*)Grid[i][1].Content)->Update(280, RGB(192, 192, 192), Msg::getMsgElement(EventID, &EventsReporter)->GetColor(), EventsReporter.GetElement(EventID)->getName());
			((Text*)Grid[i][1].Content)->GetSprite()->SetColorKey(RGB(255, 255, 255));
		}
		else
		{
			Grid[i][1].display = false;
			((Text*)Grid[i][1].Content)->SetContent(NULL);
		}
	}

	return Panel::Refresh();
}

bool Events::generate(bool waited, Cursor *cursor)
{
	// L'ajout d'un événement a été detecté
	if (EventsReporter.GetAddLine())
	{
		EventsReporter.SetAddLine(false);
		Refresh();
	}

	if (Selector->Slide(cursor))
		Refresh();

	return Panel::generate(waited, cursor);
}
