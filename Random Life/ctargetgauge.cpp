#include <cvisuallist.h>
#include "ctargetgauge.h"
#include "cinventory.h"
#include "cmagiclearner.h"
#include "cdialogue.h"
#include "../RLRes/RLRes/resource.h"

extern Display *Screen;
extern MsgReporter EventsReporter;
extern List SoundPack;
extern VisualList ZoneCharPack;
extern Inventory *PTargetInventory;
extern MagicLearner *PGreatMago;
extern Dialogue *PDialogue;

TargetGauge::TargetGauge()
{
	PRange = new Panel();
	PActions = new Panel();
	Focus = NULL;
	Observer = NULL;
}

TargetGauge::TargetGauge(int Panel_ColSpace, int Panel_RowSpace, int Panel_BorderSpace) :
Panel(2, 8, Panel_ColSpace, Panel_RowSpace, Panel_BorderSpace, true, Text(), Str()),
Item(NULL, NULL, NULL, NULL, "JAUGE CIBLE")
{
	PRange = new Panel(1, 1, 0, 0, 0, false, Text(), Str());
	PActions = new Panel(TOTAL_COMBAT_PHASE, 1, 0, 0, 0, false, Text(), Str());
	Focus = NULL;
	Observer = NULL;
}

void TargetGauge::Init()
{
	Grid[6][1].data = VISUAL_INHERIT;
	Grid[7][0].data = VISUAL;
	Grid[7][0].PicFrame = 4;
	Grid[7][0].ColSpan = -1;
	Grid[7][0].halign = HALIGN_CENTER;

	Gauge::Init();
	PRange->Init();

	Grid[6][1].Content = PRange;

	AddForbidden1(((Item*)Grid[7][0].Content));
}

void TargetGauge::Initialize()
{
	Gauge::Initialize();
	PRange->Initialize();
}

void TargetGauge::initAni()
{
	Gauge::initAni();
	
	GetVisualData(0, 7)->addAni(IDB_ACTION_BUTTONS, "", 16, 60, 16, -1, 90);

	InitCloseButton(IDB_PANEL_CLOSE, "", 16, 16);
}

void TargetGauge::restoreAni()
{	
	Gauge::restoreAni();
	
	GetVisualData(0, 7)->restoreAni();
}

void TargetGauge::destroyAni()
{
	Gauge::destroyAni();
	
	GetVisualData(0, 7)->destroyAni();
}

void TargetGauge::DestroyText()
{
	Gauge::DestroyText();
	PRange->DestroyText();
}

bool TargetGauge::Refresh()
{
	Humanized *pHumanized = dynamic_cast<Humanized*>(Focus);
	Pnj *pPnj = dynamic_cast<Pnj*>(Focus);

	if ((Focus == NULL) | (Observer == NULL))
		return false;

	// Textes
	Screen->LoadText(((Text*)Grid[6][0].Content)->GetSprite(), NULL, "Distance:", RGB(255, 255, 255), RGB(0, 0, 0));
	((Text*)Grid[6][0].Content)->GetSprite()->SetColorKey(RGB(255, 255, 255));

	// Frames
	if (!Focus->GetStatus()->dead)
	{
		if (pPnj)
		{
			switch (pPnj->GetPNJJob())
			{
			case TRADER:
				Grid[7][0].PicFrame = 9;
				break;
			case MAGIC_LEARNER:
				Grid[7][0].PicFrame = 7;
				break;
			case QUEST_GIVER:
				Grid[7][0].PicFrame = 13;
				break;
			default:
				Grid[7][0].PicFrame = 4;
				break;
			}
		}
		else
			Grid[7][0].PicFrame = 4;
	}
	else
		if (pHumanized)
			if (pHumanized->IsEmpty())
				Grid[7][0].PicFrame = 2;
			else
				Grid[7][0].PicFrame = 12;
		else
			Grid[7][0].PicFrame = 2;

	PRange->Refresh();

	return Gauge::Refresh();
}

bool TargetGauge::generate(bool waited, Cursor *cursor)
{
	Humanized *pHumanized = dynamic_cast<Humanized*>(Focus);
	Pnj *pPnj = dynamic_cast<Pnj*>(Focus);
	TileIA *pTileIA = dynamic_cast<TileIA*>(Focus);

	if ((Focus == NULL) | (Observer == NULL))
		return false;

	// Evénements sur le bouton d'action
	if (ClickOnGrid(0, 7, waited, cursor))
		if (!Focus->GetStatus()->dead)
		{
			if (!Observer->GetStatus()->fight)
			{
				if (pPnj)
				{
					if ((pPnj->GetStatus()->access == true) && (pPnj->GetStatus()->fight == false))
					{
						if (Observer->GetAdjacent(pPnj))
						{
							switch (pPnj->GetPNJJob())
							{
							case TRADER:
								*PTargetInventory->getPosition() = Axe(pPnj->getPosition()->x, pPnj->getPosition()->y + pPnj->getDimension()->y);
								PTargetInventory->show();
								PTargetInventory->Refresh();
								Focus->SetTarget(Observer);
								pTileIA->Face(Observer);
								break;
							case MAGIC_LEARNER:
								*PGreatMago->getPosition() = Axe(pPnj->getPosition()->x, pPnj->getPosition()->y + pPnj->getDimension()->y);
								PGreatMago->show();
								PGreatMago->Refresh();
								Focus->SetTarget(Observer);
								pTileIA->Face(Observer);
								break;
							case QUEST_GIVER:
								PDialogue->SetSpeaker(pPnj);
								PDialogue->SetListener(Observer);
								*PDialogue->getPosition() = Axe(pPnj->getPosition()->x, pPnj->getPosition()->y + pPnj->getDimension()->y);
								PDialogue->show();
								PDialogue->Refresh();
								Focus->SetTarget(Observer);
								pTileIA->Face(Observer);
								break;
							default:
								Observer->Spy();
								break;
							}
						}
						else
							if (pPnj->GetPNJJob())
							{
								pPnj->PlaySound(Sound::getSoundElement(ACTION_FORBIDDEN, &SoundPack), 0, false);
								EventsReporter.AddMessage(ACTION_DENIED, 4);
							}
							else
								Observer->Spy();
					}
					else
						if (pPnj->GetPNJJob())
						{
							pPnj->PlaySound(Sound::getSoundElement(ACTION_FORBIDDEN, &SoundPack), 0, false);
							EventsReporter.AddMessage(ACTION_DENIED, 10);
						}
						else
							Observer->Spy();
				}
				else
					if (Focus->GetRace() != ANIMAL)
						Observer->Spy();
					else
					{
						Observer->PlaySound(Sound::getSoundElement(ACTION_FORBIDDEN, &SoundPack), 0, false);
						EventsReporter.AddMessage(ACTION_DENIED, 5);
					}
			}
			else
			{
				Observer->PlaySound(Sound::getSoundElement(ACTION_FORBIDDEN, &SoundPack), 0, false);
				EventsReporter.AddMessage(ACTION_DENIED, 6);
			}
		}
		else
			if (Observer->GetAdjacent(Focus))
			{
				if (pHumanized)
				{
					if (pHumanized->IsEmpty())
					{
						hide();
						pHumanized->remove();
						ZoneCharPack.RemoveElement(pHumanized);
					}
					else
					{
						*PTargetInventory->getPosition() = Axe(pHumanized->getPosition()->x, pHumanized->getPosition()->y + pHumanized->getDimension()->y);
						PTargetInventory->show();
						PTargetInventory->Refresh();
					}
				}
				else
				{
					Focus->remove();
					ZoneCharPack.RemoveElement(Focus);
				}
			}
			else
			{
				Observer->PlaySound(Sound::getSoundElement(ACTION_FORBIDDEN, &SoundPack), 0, false);
				EventsReporter.AddMessage(ACTION_DENIED, 4);
			}
	
	
	return Panel::generate(waited, cursor);
}

bool TargetGauge::Run(Item *Cursor)
{
	if (Focus && Observer)
	{
		// Distance de la cible
		Screen->LoadText(PRange->GetTextData(0, 0)->GetSprite(), NULL, Observer->GetRange(Focus->GetTileNodeID()), RGB(255, 255, 255), RGB(0, 0, 0));
		PRange->GetTextData(0, 0)->GetSprite()->SetColorKey(RGB(255, 255, 255));
	}

	return Panel::Run(Cursor);
}
