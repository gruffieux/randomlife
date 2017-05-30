#include "cstatsinfo.h"
#include "cgauge.h"
#include "caskforsave.h"

extern CharCombat CombatData[TOTAL_COMBAT_RECORD];
extern Display *Screen;
extern MsgReporter EventsReporter;
extern Gauge *PGauge;
extern AskForSave *PSave;

StatsInfo::StatsInfo()
{
	magic = false;
	Mother = NULL;
	Focus = NULL;
}

StatsInfo::StatsInfo(bool SI_magic, int Panel_ColCount, int Panel_RowCount, int Panel_ColSpace, int Panel_RowSpace, int Panel_BorderSpace, int Item_width, int Item_height, Str Object_Name) :
Panel(Panel_ColCount, Panel_RowCount, Panel_ColSpace, Panel_RowSpace, Panel_BorderSpace, false, Text(), NULL, NULL, Str()),
Item(NULL, NULL, Item_width, Item_height, Object_Name)
{
	magic = SI_magic;
	Mother = NULL;
	Focus = NULL;
}

bool StatsInfo::Run(Item *Cursor)
{
	if (Focus) return Panel::Run(Cursor);

	return false;
}

bool StatsInfo::Refresh()
{
	int x, y, *DisplayedScore;

	if (!Focus) return false;

	if (magic)
	{
		Screen->LoadText(((Text*)Grid[0][0].Content)->GetSprite(), NULL, name, RGB(255, 255, 255), RGB(0, 0, 0));
		((Text*)Grid[0][0].Content)->GetSprite()->SetColorKey(RGB(255, 255, 255));
		Screen->LoadText(((Text*)Grid[1][0].Content)->GetSprite(), NULL, "Nom", RGB(255, 255, 255), RGB(0, 0, 0));
		((Text*)Grid[1][0].Content)->GetSprite()->SetColorKey(RGB(255, 255, 255));
		Screen->LoadText(((Text*)Grid[1][3].Content)->GetSprite(), NULL, "Score", RGB(255, 255, 255), RGB(0, 0, 0));
		((Text*)Grid[1][3].Content)->GetSprite()->SetColorKey(RGB(255, 255, 255));
		for (y = 2; y < Focus->GetMagicSchoolPack()->GetElementCount() + 2; y++)
		{
			x = 0;
			Screen->LoadText(((Text*)Grid[y][x].Content)->GetSprite(), NULL, Focus->GetMagicSchoolPack()->getCharStatElement(y-2)->GetDescription(), RGB(255, 255, 255), RGB(0, 0, 0));
			((Text*)Grid[y][x].Content)->GetSprite()->SetColorKey(RGB(255, 255, 255));
			x = 3;
			if (Focus->GetMagicSchoolPack()->getCharStatElement(y-2)->GetTempScore())
				DisplayedScore = Focus->GetMagicSchoolPack()->getCharStatElement(y-2)->GetpTempScore();
			else
				DisplayedScore = Focus->GetMagicSchoolPack()->getCharStatElement(y-2)->GetpScore();
			Screen->LoadText(((Text*)Grid[y][x].Content)->GetSprite(), NULL, BuildString("%d+(%d)", *DisplayedScore, Focus->GetMagicSchoolPack()->getCharStatElement(y-2)->GetModifier()), RGB(255, 255, 255), RGB(0, 0, 0));
			((Text*)Grid[y][x].Content)->GetSprite()->SetColorKey(RGB(255, 255, 255));
		}
		Screen->LoadText(((Text*)Grid[5][1].Content)->GetSprite(), NULL, BuildString("Magic Points: %d", Focus->GetMagicPoints()), RGB(255, 255, 255), RGB(0, 0, 0));
		((Text*)Grid[5][1].Content)->GetSprite()->SetColorKey(RGB(255, 255, 255));
	}
	else
	{
		Screen->LoadText(((Text*)Grid[0][0].Content)->GetSprite(), NULL, name, RGB(255, 255, 255), RGB(0, 0, 0));
		((Text*)Grid[0][0].Content)->GetSprite()->SetColorKey(RGB(255, 255, 255));
		Screen->LoadText(((Text*)Grid[1][0].Content)->GetSprite(), NULL, "Nom", RGB(255, 255, 255), RGB(0, 0, 0));
		((Text*)Grid[1][0].Content)->GetSprite()->SetColorKey(RGB(255, 255, 255));
		Screen->LoadText(((Text*)Grid[1][3].Content)->GetSprite(), NULL, "Score", RGB(255, 255, 255), RGB(0, 0, 0));
		((Text*)Grid[1][3].Content)->GetSprite()->SetColorKey(RGB(255, 255, 255));
		for (y = 2; y < Focus->GetStatPack()->GetElementCount() + 2; y++)
		{
			x = 0;
			Screen->LoadText(((Text*)Grid[y][x].Content)->GetSprite(), NULL, BuildString("(%s) %s", Focus->GetStatPack()->getCharStatElement(y-2)->GetLabel().Get(), Focus->GetStatPack()->getCharStatElement(y-2)->GetDescription().Get()), RGB(255, 255, 255), RGB(0, 0, 0));
			((Text*)Grid[y][x].Content)->GetSprite()->SetColorKey(RGB(255, 255, 255));
			x = 3;
			if (Focus->GetStatPack()->getCharStatElement(y-2)->GetTempScore())
				DisplayedScore = Focus->GetStatPack()->getCharStatElement(y-2)->GetpTempScore();
			else
				DisplayedScore = Focus->GetStatPack()->getCharStatElement(y-2)->GetpScore();
			Screen->LoadText(((Text*)Grid[y][x].Content)->GetSprite(), NULL, BuildString("%d+(%d)", *DisplayedScore, Focus->GetStatPack()->getCharStatElement(y-2)->GetModifier()), RGB(255, 255, 255), RGB(0, 0, 0));
			((Text*)Grid[y][x].Content)->GetSprite()->SetColorKey(RGB(255, 255, 255));
		}
		Screen->LoadText(((Text*)Grid[9][1].Content)->GetSprite(), NULL, BuildString("Skill Points: %d", Focus->GetSkillPoints()), RGB(255, 255, 255), RGB(0, 0, 0));
		((Text*)Grid[9][1].Content)->GetSprite()->SetColorKey(RGB(255, 255, 255));
	}

	return Panel::Refresh();
}

bool StatsInfo::generate(bool waited, Cursor *cursor)
{
	int i, *TempScore, *CurrentScore, *points;
	bool validate = false;
	Str Description;
	CharStatList *Stats;

	if (!Focus) return false;

	// On set les pointeurs
	if (magic)
	{
		points = Focus->GetpMagicPoints();
		Stats = Focus->GetMagicSchoolPack();
	}
	else
	{
		points = Focus->GetpSkillPoints();
		Stats = Focus->GetStatPack();
	}

	// Evénements (augmentation/diminution des stats, validation)
	for (i = 0; i < Stats->GetElementCount(); i++)
	{
		TempScore = Stats->getCharStatElement(i)->GetpTempScore();
		CurrentScore = Stats->getCharStatElement(i)->GetpScore();
		Description = Stats->getCharStatElement(i)->GetDescription();

		if (*TempScore)
		{
			Grid[i+2][1].display = true;
			Grid[i+2][2].display = true;
		}
		else
		{
			Grid[i+2][1].display = false;
			Grid[i+2][2].display = false;
		}

		if ((ClickOnGrid(1, i+2, waited, cursor) == true) && (*points >= STAT_PRICE) && (*TempScore < MAX_CHAR_STAT_SCORE))
		{
			*TempScore += 1;
			*points -= STAT_PRICE;
			Refresh();
		}

		if ((ClickOnGrid(2, i+2, waited, cursor) == true) && (*TempScore > *CurrentScore))
		{
			*TempScore -= 1;
			*points += STAT_PRICE;
			Refresh();
		}

		if (ClickOnGrid(0, RowCount-1, waited, cursor))
		{
			if (*TempScore)
			{
				Focus->RemoveAllStatMalus();
				if (*TempScore > *CurrentScore)
					validate = true;
				int score = *TempScore - *CurrentScore;
				int cost = score * STAT_PRICE;
				*CurrentScore = *TempScore;
				if (*points == 0)
					*TempScore = 0;
				if (score)
				{
					if (magic)
						EventsReporter.AddMessage(CURRENT_ACTION, BuildString("Vous améliorez votre pouvoir %s de %d pour %d points magiques", Description.Get(), score, cost), PROGRESS_MSG_COLOR);
					else
						EventsReporter.AddMessage(CURRENT_ACTION, BuildString("Vous améliorez votre %s de %d pour %d points de compétences", Description.Get(), score, cost), PROGRESS_MSG_COLOR);
				}
				else
				{
					if (magic)
						EventsReporter.AddMessage(CURRENT_ACTION, BuildString("Vous n'améliorez pas votre pouvoir %s", Description.Get()), PROGRESS_MSG_COLOR);
					else
						EventsReporter.AddMessage(CURRENT_ACTION, BuildString("Vous n'améliorez pas votre %s", Description.Get()), PROGRESS_MSG_COLOR);
				}
				Stats->getCharStatElement(i)->SetModifier();
				Focus->AddAllStatMalus();
				Refresh();
			}
		}
	}

	if (validate)
	{
		Focus->GenerateStat2();
		Focus->GenerateEquipment();
		Focus->UpdateMagicSpell();
		if (Focus->UpdateCombat(CombatData))
			PGauge->Refresh();
		if (!Focus->GetStatus()->fight)
		{
			PSave->centerPosition(Axe(Screen->GetCurrentMode()->GetWidth(), Screen->GetCurrentMode()->GetHeight()));
			PSave->SetRemainingPoints(Focus->GetSkillPoints(), Focus->GetMagicPoints());
			PSave->show();
			PSave->Refresh();
			Mother->hide();
		}
	}

	return Panel::generate(waited, cursor);
}
