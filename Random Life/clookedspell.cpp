#include "clookedspell.h"
#include "caskforsave.h"
#include "cquestbook.h"
#include "cworldtime.h"
#include "ccharinfo.h"
#include "cspelllist.h"
#include "../RLRes/RLRes/resource.h"

extern Display *Screen;
extern MsgReporter EventsReporter;
extern List SoundPack;
extern AskForSave *PSave;
extern QuestBook *PBook;
extern WorldTime *PWorldTime;
extern CharInfo *PCharInfo;
extern SpellList *PSpellList;
extern Pnj *GreatMago;

LookedSpell::LookedSpell()
{
	learn = false;
	Focus = NULL;
	Observer = NULL;
}

LookedSpell::LookedSpell(int Panel_ColSpace, int Panel_RowSpace, int Panel_BorderSpace) :
Panel(8, 8, Panel_ColSpace, Panel_RowSpace, Panel_BorderSpace, true, Text(), Str()),
Item(NULL, NULL, NULL, NULL, "SORT MAGIQUE")
{
	learn = false;
	Focus = NULL;
	Observer = NULL;
}

void LookedSpell::Init()
{
	int i;

	Grid[0][0].ColSpan = Grid[1][0].ColSpan = -1;
	Grid[0][0].halign = Grid[1][0].halign = HALIGN_CENTER;
	Grid[7][0].data = VISUAL;
	Grid[6][1].data = VISUAL;
	Grid[6][3].data = VISUAL;
	for (i = 3; i < RowCount - 1; i++)
		Grid[i][7].data = VISUAL;
	
	Grid[7][0].PicFrame = 8;
	Grid[6][1].PicFrame = 0;
	Grid[6][3].PicFrame = 1;

	AlignCols();

	Panel::Init();
	
	AddForbidden1(GetVisualData(0, 7));
	AddForbidden1(GetVisualData(1, 6));
	AddForbidden1(GetVisualData(3, 6));
	for (i = 3; i < RowCount - 1; i++)
		AddForbidden1(GetVisualData(7, i));
}

void LookedSpell::initAni()
{
	int i;
	
	GetVisualData(1, 6)->addAni(IDB_SETTING_BUTTONS, "", 4, 16, 16, -1, 90);
	GetVisualData(3, 6)->addAni(IDB_SETTING_BUTTONS, "", 4, 16, 16, -1, 90);
	
	for (i = 3; i < RowCount - 1; i++)
		GetVisualData(7, i)->addAni(IDB_FLAG_BUTTONS, "", 2, 16, 16, -1, 90);
	
	GetVisualData(0, 7)->addAni(IDB_ACTION_BUTTONS, "", 16, 60, 16, -1, 90);

	InitCloseButton(IDB_PANEL_CLOSE, "", 16, 16);
}

void LookedSpell::restoreAni()
{
	int i;
	
	Panel::restoreAni();
	
	GetVisualData(1, 6)->restoreAni();
	GetVisualData(3, 6)->restoreAni();
	
	for (i = 3; i < RowCount - 1; i++)
		GetVisualData(7, i)->restoreAni();
	
	GetVisualData(0, 7)->restoreAni();
}

void LookedSpell::destroyAni()
{
	int i;
	
	Panel::destroyAni();
	
	GetVisualData(1, 6)->destroyAni();
	GetVisualData(3, 6)->destroyAni();
	
	for (i = 3; i < RowCount - 1; i++)
		GetVisualData(7, i)->destroyAni();
	
	GetVisualData(0, 7)->destroyAni();
}

bool LookedSpell::Run(Item *Cursor)
{
	if (Focus) return Panel::Run(Cursor);

	return false;
}

bool LookedSpell::Refresh()
{
	int i;

	if ((Focus == NULL) | (Observer == NULL))
		return false;
	
	// Affichage des textes
	Screen->LoadText(((Text*)Grid[0][0].Content)->GetSprite(), NULL, name, RGB(255, 255, 255), RGB(0, 0, 0));
	((Text*)Grid[0][0].Content)->GetSprite()->SetColorKey(RGB(255, 255, 255));
	Screen->LoadText(((Text*)Grid[1][0].Content)->GetSprite(), NULL, Focus->GetDescription(), RGB(255, 255, 255), RGB(0, 0, 0));
	((Text*)Grid[1][0].Content)->GetSprite()->SetColorKey(RGB(255, 255, 255));
	Screen->LoadText(((Text*)Grid[2][0].Content)->GetSprite(), NULL, "Niveau:", RGB(255, 255, 255), RGB(0, 0, 0));
	((Text*)Grid[2][0].Content)->GetSprite()->SetColorKey(RGB(255, 255, 255));
	Screen->LoadText(((Text*)Grid[3][0].Content)->GetSprite(), NULL, "Difficulté:", RGB(255, 255, 255), RGB(0, 0, 0));
	((Text*)Grid[3][0].Content)->GetSprite()->SetColorKey(RGB(255, 255, 255));
	Screen->LoadText(((Text*)Grid[4][0].Content)->GetSprite(), NULL, "Difficulté min:", RGB(255, 255, 255), RGB(0, 0, 0));
	((Text*)Grid[4][0].Content)->GetSprite()->SetColorKey(RGB(255, 255, 255));
	Screen->LoadText(((Text*)Grid[5][0].Content)->GetSprite(), NULL, "Temps", RGB(255, 255, 255), RGB(0, 0, 0));
	((Text*)Grid[5][0].Content)->GetSprite()->SetColorKey(RGB(255, 255, 255));
	Screen->LoadText(((Text*)Grid[6][0].Content)->GetSprite(), NULL, "d'incantation:", RGB(255, 255, 255), RGB(0, 0, 0));
	((Text*)Grid[6][0].Content)->GetSprite()->SetColorKey(RGB(255, 255, 255));
	Screen->LoadText(((Text*)Grid[2][2].Content)->GetSprite(), NULL, Focus->GetLevel(), RGB(255, 255, 255), RGB(0, 0, 0));
	((Text*)Grid[2][2].Content)->GetSprite()->SetColorKey(RGB(255, 255, 255));
	Screen->LoadText(((Text*)Grid[3][2].Content)->GetSprite(), NULL, Focus->GetDifficulty(), RGB(255, 255, 255), RGB(0, 0, 0));
	((Text*)Grid[3][2].Content)->GetSprite()->SetColorKey(RGB(255, 255, 255));
	Screen->LoadText(((Text*)Grid[4][2].Content)->GetSprite(), NULL, Focus->GetMinDifficulty(), RGB(255, 255, 255), RGB(0, 0, 0));
	((Text*)Grid[4][2].Content)->GetSprite()->SetColorKey(RGB(255, 255, 255));
	Screen->LoadText(((Text*)Grid[6][2].Content)->GetSprite(), NULL, Focus->GetCastTime(), RGB(255, 255, 255), RGB(0, 0, 0));
	((Text*)Grid[6][2].Content)->GetSprite()->SetColorKey(RGB(255, 255, 255));
	Screen->LoadText(((Text*)Grid[2][4].Content)->GetSprite(), NULL, "Coût en Mana (perso):", RGB(255, 255, 255), RGB(0, 0, 0));
	((Text*)Grid[2][4].Content)->GetSprite()->SetColorKey(RGB(255, 255, 255));
	Screen->LoadText(((Text*)Grid[3][4].Content)->GetSprite(), NULL, "Durée de l'effet:", RGB(255, 255, 255), RGB(0, 0, 0));
	((Text*)Grid[3][4].Content)->GetSprite()->SetColorKey(RGB(255, 255, 255));
	Screen->LoadText(((Text*)Grid[4][4].Content)->GetSprite(), NULL, "Distance max:", RGB(255, 255, 255), RGB(0, 0, 0));
	((Text*)Grid[4][4].Content)->GetSprite()->SetColorKey(RGB(255, 255, 255));
	Screen->LoadText(((Text*)Grid[5][4].Content)->GetSprite(), NULL, "Effet min:", RGB(255, 255, 255), RGB(0, 0, 0));
	((Text*)Grid[5][4].Content)->GetSprite()->SetColorKey(RGB(255, 255, 255));
	Screen->LoadText(((Text*)Grid[6][4].Content)->GetSprite(), NULL, "Effet max", RGB(255, 255, 255), RGB(0, 0, 0));
	((Text*)Grid[6][4].Content)->GetSprite()->SetColorKey(RGB(255, 255, 255));
	Screen->LoadText(((Text*)Grid[2][6].Content)->GetSprite(), NULL, BuildString("%d (%d)", Focus->GetManaCost(Observer->GetStatPack()->GetStat(WILLPOWER)->GetModifier(), false).RealScore, Focus->GetManaCost(Observer->GetStatPack()->GetStat(WILLPOWER)->GetModifier(), false).score), RGB(255, 255, 255), RGB(0, 0, 0));
	((Text*)Grid[2][6].Content)->GetSprite()->SetColorKey(RGB(255, 255, 255));
	Screen->LoadText(((Text*)Grid[3][6].Content)->GetSprite(), NULL, Focus->GetDuration()->data, RGB(255, 255, 255), RGB(0, 0, 0));
	((Text*)Grid[3][6].Content)->GetSprite()->SetColorKey(RGB(255, 255, 255));
	Screen->LoadText(((Text*)Grid[4][6].Content)->GetSprite(), NULL, Focus->GetRange()->data, RGB(255, 255, 255), RGB(0, 0, 0));
	((Text*)Grid[4][6].Content)->GetSprite()->SetColorKey(RGB(255, 255, 255));
	Screen->LoadText(((Text*)Grid[5][6].Content)->GetSprite(), NULL, Focus->GetMinEffect()->data, RGB(255, 255, 255), RGB(0, 0, 0));
	((Text*)Grid[5][6].Content)->GetSprite()->SetColorKey(RGB(255, 255, 255));
	Screen->LoadText(((Text*)Grid[6][6].Content)->GetSprite(), NULL, Focus->GetMaxEffect()->data, RGB(255, 255, 255), RGB(0, 0, 0));
	((Text*)Grid[6][6].Content)->GetSprite()->SetColorKey(RGB(255, 255, 255));
	
	// Frames
	if (learn)
	{
		Grid[7][0].PicFrame = 7;
		Grid[6][1].display = false;
		Grid[6][3].display = false;
		for (i = 3; i < RowCount - 1; i++)
			Grid[i][7].display = false;
	}
	else
	{
		Grid[7][0].PicFrame = 8;
		if (Focus->GetDuration()->flag)
			Grid[3][7].PicFrame = 1;
		else
			Grid[3][7].PicFrame = 0;
		if (Focus->GetRange()->flag)
			Grid[4][7].PicFrame = 1;
		else
			Grid[4][7].PicFrame = 0;
		if (Focus->GetMinEffect()->flag)
			Grid[5][7].PicFrame = 1;
		else
			Grid[5][7].PicFrame = 0;
		if (Focus->GetMaxEffect()->flag)
			Grid[6][7].PicFrame = 1;
		else
			Grid[6][7].PicFrame = 0;
		Grid[6][1].display = true;
		Grid[6][3].display = true;
		for (i = 3; i < RowCount - 1; i++)
			Grid[i][7].display = true;
	}

	return Panel::Refresh();
}

bool LookedSpell::generate(bool waited, Cursor *cursor)
{
	int i;

	if ((Focus == NULL) | (Observer == NULL))
		return false;

	// Evénements sur le bouton pour diminuer le temps d'incantation
	if (ClickOnGrid(1, 6, waited, cursor))
	{
		Focus->DecreaseCastTime();
		Focus->GetManaCost(Observer->GetStatPack()->GetStat(WILLPOWER)->GetModifier(), true);
		Refresh();
	}

	// Evénements sur le bouton pour augmenter le temps d'incantation
	if (ClickOnGrid(3, 6, waited, cursor))
	{
		Focus->IncreaseCastTime();
		Focus->GetManaCost(Observer->GetStatPack()->GetStat(WILLPOWER)->GetModifier(), true);
		Refresh();
	}

	// Evénements sur les boutons de flags
	for (i = 3; i < RowCount - 1; i++)
		if (ClickOnGrid(7, i, waited, cursor))
		{
			Focus->GetDuration()->flag = Focus->GetRange()->flag = Focus->GetMinEffect()->flag = Focus->GetMaxEffect()->flag = false;
			switch (i)
			{
			case 3:
				Focus->GetDuration()->flag = true;
				break;
			case 4:
				Focus->GetRange()->flag = true;
				break;
			case 5:
				Focus->GetMinEffect()->flag = true;
				break;
			case 6:
				Focus->GetMaxEffect()->flag = true;
				break;
			}
			Refresh();
		}

	// Evénements sur le bouton d'action (apprendre ou mémoriser le sort)
	if (ClickOnGrid(0, 7, waited, cursor))
	{
		if (learn)
		{
			if (Observer->LearnSpell(Focus, *GreatMago->GetMagicSpellPack(Focus->GetSchool()-1), &EventsReporter) == ACTION_SUCCESS)
			{
				Sound::getSoundElement(SPELL_LEARNT, &SoundPack)->play(DMUS_SEGF_SECONDARY);
				if ((PBook->GetStatus(PRIEST_QUEST) == 1) && (Focus->GetEffect() == SELF_HEAL) && (Focus->GetLevel() == 12))
				{
					PBook->SetMaxHeal(true);
					PBook->AddComment(PWorldTime->GetDay(), true, PRIEST_STEP2, &EventsReporter);
				}
				PSave->centerPosition(Axe(Screen->GetCurrentMode()->GetWidth(), Screen->GetCurrentMode()->GetHeight()));
				PSave->SetRemainingPoints(Observer->GetSkillPoints(), Observer->GetMagicPoints());
				PSave->show();
				PSave->Refresh();
				PCharInfo->hide();
				PSpellList->Refresh();
				PCharInfo->GetPMagicSchools()->Refresh();
			}
		}
		else
		{
			Observer->SetMemorizedSpell(Focus);
			EventsReporter.AddMessage(CURRENT_ACTION, BuildString("Vous mémorizez le sort %s", Focus->GetDescription().Get()), CAST_SPELL_MSG_COLOR);
		}

		hide();
		Focus = NULL;
	}

	return Panel::generate(waited, cursor);
}
