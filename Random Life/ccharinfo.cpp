#include "ccharinfo.h"
#include "../RLRes/RLRes/resource.h"

extern Display *Screen;

CharInfo::CharInfo()
{
	Focus = NULL;
	PSkills = new Panel();
	PCombatSkills = new Panel();
	PMagicSkill = new Panel();
	PStats = new StatsInfo();
	PStats2 = new Panel();
	PMagicSchools = new StatsInfo();
}

CharInfo::CharInfo(int Panel_ColSpace, int Panel_RowSpace, int Panel_BorderSpace) :
Panel(3, 3, Panel_ColSpace, Panel_RowSpace, Panel_BorderSpace, true, Text(), Str()),
Item(NULL, NULL, NULL, NULL, "MON PERSONNAGE")
{
	Focus = NULL;
	PSkills = new Panel(3, 9, 5, 2, 6, false, Text(0, RGB(0, 128, 255)), 240, 192, "COMPETENCES");
	PCombatSkills = new Panel(3, 7, 5, 2, 6, false, Text(0, RGB(0, 128, 255)), 240, 192, "COMPETENCES DE COMBAT");
	PMagicSkill = new Panel(3, 3, 5, 2, 6, false, Text(), 240, 192, "COMPETENCE DE MAGIE");
	PStats = new StatsInfo(false, 4, 10, 5, 2, 6, 240, 192, "CARACTERISTIQUES");
	PStats2 = new Panel(2, 8, 5, 2, 6, false, Text(0, RGB(0, 128, 255)), 240, 192, "CARACTERISTIQUES 2");
	PMagicSchools = new StatsInfo(true, 4, 6, 5, 2, 6, 240, 192, "ECOLES DE MAGIE");
}

CharInfo::~CharInfo()
{
	delete PSkills;
	delete PCombatSkills;
	delete PMagicSkill;
	delete PStats;
	delete PStats2;
	delete PMagicSchools;
}

void CharInfo::SetFocus(Character *CharInfo_Focus)
{
	Focus = CharInfo_Focus;
	Humanized *pHumanized = dynamic_cast<Humanized*>(Focus);

	if (pHumanized)
	{
		Grid[0][0].display = true;
		Grid[0][2].display = true;
		Grid[1][2].display = true;
	}
	else
	{
		Grid[0][0].display = false;
		Grid[0][2].display = false;
		Grid[1][2].display = false;
	}
}

void CharInfo::Init()
{
	int i, j;
	Humanized *pHumanized = dynamic_cast<Humanized*>(Focus);
	Player *pPlayer = dynamic_cast<Player*>(Focus);

	Grid[0][0].ColSpan = -1;
	Grid[0][0].halign = HALIGN_CENTER;

	PSkills->GetGrid(0, 0)->ColSpan = -1;
	PSkills->GetGrid(0, 0)->halign = HALIGN_CENTER;
	for (i = 1; i < PSkills->GetRowCount(); i++)
		PSkills->AlignColsOnRow(i);
	PSkills->AlignColContent(2, HALIGN_CENTER);

	PCombatSkills->GetGrid(0, 0)->ColSpan = -1;
	PCombatSkills->GetGrid(0, 0)->halign = HALIGN_CENTER;
	for (i = 1; i < PCombatSkills->GetRowCount(); i++)
		PCombatSkills->AlignColsOnRow(i);
	PCombatSkills->AlignColContent(2, HALIGN_CENTER);

	PMagicSkill->GetGrid(0, 0)->ColSpan = -1;
	PMagicSkill->GetGrid(0, 0)->halign = HALIGN_CENTER;
	PMagicSkill->AlignColsOnRow(1);
	PMagicSkill->AlignColContent(2, HALIGN_CENTER);

	PStats->GetGrid(0, 0)->ColSpan = -1;
	PStats->GetGrid(0, 0)->halign = HALIGN_CENTER;
	PStats->GetGrid(1, PStats->GetRowCount()-1)->ColSpan = 3;
	PStats->AlignColContent(3, HALIGN_CENTER);
	for (i = 1; i < PStats->GetRowCount()-1; i++)
		PStats->AlignColsOnRow(i);

	PStats2->GetGrid(0, 0)->ColSpan = -1;
	PStats2->GetGrid(0, 0)->halign = HALIGN_CENTER;
	PStats2->GetGrid(1, PStats2->GetRowCount()-1)->ColSpan = 3;
	PStats2->AlignColContent(1, HALIGN_CENTER);
	for (i = 1; i < PStats2->GetRowCount(); i++)
		PStats2->AlignColsOnRow(i);

	PMagicSchools->GetGrid(0, 0)->ColSpan = -1;
	PMagicSchools->GetGrid(0, 0)->halign = HALIGN_CENTER;
	PMagicSchools->GetGrid(1, PMagicSchools->GetRowCount()-1)->ColSpan = 3;
	PMagicSchools->AlignColContent(3, HALIGN_CENTER);
	for (i = 1; i < PMagicSchools->GetRowCount()-1; i++)
		PMagicSchools->AlignColsOnRow(i);

	PStats->SetMother(this);
	PStats->SetFocus(pPlayer);
	PMagicSchools->SetMother(this);
	PMagicSchools->SetFocus(pPlayer);

	for (i = 0; i < Focus->GetCombatSkillPack()->GetElementCount(); i++)
	{
		PCombatSkills->GetGrid(1, i+2)->data = VISUAL_INHERIT;
		PCombatSkills->GetGrid(0, i+2)->PopupText = "Stats liées: ";
		for (j = 0; j < Focus->GetCombatSkillPack()->getCharSkillElement(i)->GetStatPack()->GetElementCount(); j++)
		{
			PCombatSkills->GetGrid(0, i+2)->popup = true;
			PCombatSkills->GetGrid(0, i+2)->PopupText = PCombatSkills->GetGrid(0, i+2)->PopupText + Focus->GetCombatSkillPack()->getCharSkillElement(i)->GetStatPack()->getCharStatElement(j)->GetLabel();
			if (Focus->GetCombatSkillPack()->getCharSkillElement(i)->GetStatPack()->ElementExist(j+1))
				PCombatSkills->GetGrid(0, i+2)->PopupText = PCombatSkills->GetGrid(0, i+2)->PopupText + (Str)", ";
		}
	}

	PStats->GetGrid(0, 9)->data = VISUAL;
	PStats->GetGrid(0, 9)->PicFrame = 6;
	for (i = 0; i < Focus->GetStatPack()->GetElementCount(); i++)
	{
		PStats->GetGrid(1, i+2)->data = VISUAL;
		PStats->GetGrid(2, i+2)->data = VISUAL;
		PStats->GetGrid(1, i+2)->PicFrame = 0;
		PStats->GetGrid(2, i+2)->PicFrame = 1;
	}

	for (i = 0; i < Focus->GetStat2Pack()->GetElementCount(); i++)
	{
		PStats2->GetGrid(0, i+2)->popup = true;
		PStats2->GetGrid(0, i+2)->PopupText = Focus->GetStat2Pack()->getCharStatElement(i)->GetLabel();
	}

	if (pHumanized)
	{
		for (i = 0; i < pHumanized->GetSkillPack()->GetElementCount(); i++)
		{
			PSkills->GetGrid(1, i+2)->data = VISUAL_INHERIT;
			PSkills->GetGrid(0, i+2)->PopupText = "Stats liées: ";
			for (j = 0; j < pHumanized->GetSkillPack()->getCharSkillElement(i)->GetStatPack()->GetElementCount(); j++)
			{
				PSkills->GetGrid(0, i+2)->popup = true;
				PSkills->GetGrid(0, i+2)->PopupText = PSkills->GetGrid(0, i+2)->PopupText + pHumanized->GetSkillPack()->getCharSkillElement(i)->GetStatPack()->getCharStatElement(j)->GetLabel();
				if (pHumanized->GetSkillPack()->getCharSkillElement(i)->GetStatPack()->ElementExist(j+1))
					PSkills->GetGrid(0, i+2)->PopupText = PSkills->GetGrid(0, i+2)->PopupText + (Str)", ";
			}
		}

		PMagicSkill->GetGrid(1, 2)->data = VISUAL_INHERIT;

		PMagicSchools->GetGrid(0, 5)->data = VISUAL;
		PMagicSchools->GetGrid(0, 5)->PicFrame = 6;
		for (i = 0; i < pHumanized->GetMagicSchoolPack()->GetElementCount(); i++)
		{
			PMagicSchools->GetGrid(1, i+2)->data = VISUAL;
			PMagicSchools->GetGrid(2, i+2)->data = VISUAL;
			PMagicSchools->GetGrid(1, i+2)->PicFrame = 0;
			PMagicSchools->GetGrid(2, i+2)->PicFrame = 1;
		}
	}

	for (i = 0; i < ColCount; i++)
		for (j = 1; j < RowCount; j++)
			Grid[j][i].data = VISUAL_INHERIT;

	Panel::Init();
	PSkills->Init();
	PCombatSkills->Init();
	PMagicSkill->Init();
	PStats->Init();
	PStats2->Init();
	PMagicSchools->Init();

	AddForbidden1(PStats->GetVisualData(0, 9));
	for (i = 0; i < Focus->GetStatPack()->GetElementCount(); i++)
	{
		AddForbidden1(PStats->GetVisualData(1, i+2));
		AddForbidden1(PStats->GetVisualData(2, i+2));
	}

	if (pHumanized)
	{
		AddForbidden1(PMagicSchools->GetVisualData(0, 5));
		for (i = 0; i < pHumanized->GetMagicSchoolPack()->GetElementCount(); i++)
		{
			AddForbidden1(PMagicSchools->GetVisualData(1, i+2));
			AddForbidden1(PMagicSchools->GetVisualData(2, i+2));
		}
	}

	if (pPlayer)
	{
		for (i = 0; i < pPlayer->GetSkillPack()->GetElementCount(); i++)
			PSkills->GetGrid(1, i+2)->Content = EnergyBar::getEnergyBarElement(i, pPlayer->GetSkillBarPack());
		for (i = 0; i < pPlayer->GetCombatSkillPack()->GetElementCount(); i++)
			PCombatSkills->GetGrid(1, i+2)->Content = EnergyBar::getEnergyBarElement(i, pPlayer->GetCombatSkillBarPack());
		PMagicSkill->GetGrid(1, 2)->Content = pPlayer->GetMagicSkillBar();
	}

	Grid[1][0].Content = PSkills;
	Grid[1][1].Content = PCombatSkills;
	Grid[1][2].Content = PMagicSkill;
	Grid[2][0].Content = PStats;
	Grid[2][1].Content = PStats2;
	Grid[2][2].Content = PMagicSchools;
}

void CharInfo::Initialize()
{
	Panel::Initialize();
	PSkills->Initialize();
	PCombatSkills->Initialize();
	PMagicSkill->Initialize();
	PStats->Initialize();
	PStats2->Initialize();
	PMagicSchools->Initialize();
}

void CharInfo::initAni()
{
	int i;
	Humanized *pHumanized = dynamic_cast<Humanized*>(Focus);
	Player *pPlayer = dynamic_cast<Player*>(Focus);

	PStats->GetVisualData(0, 9)->addAni(IDB_ACTION_BUTTONS, "", 16, 60, 16, -1, 90);
	for (i = 0; i < Focus->GetStatPack()->GetElementCount(); i++)
	{
		PStats->GetVisualData(1, i+2)->addAni(IDB_PROGRESS_BUTTONS, "", 2, 16, 16, -1, 90);
		PStats->GetVisualData(2, i+2)->addAni(IDB_PROGRESS_BUTTONS, "", 2, 16, 16, -1, 90);
	}

	if (pHumanized)
	{
		PMagicSchools->GetVisualData(0, 5)->addAni(IDB_ACTION_BUTTONS, "", 16, 60, 16, -1, 90);
		for (i = 0; i < pHumanized->GetMagicSchoolPack()->GetElementCount(); i++)
		{
			PMagicSchools->GetVisualData(1, i+2)->addAni(IDB_PROGRESS_BUTTONS, "", 2, 16, 16, -1, 90);
			PMagicSchools->GetVisualData(2, i+2)->addAni(IDB_PROGRESS_BUTTONS, "", 2, 16, 16, -1, 90);
		}
	}

	if (pPlayer)
	{
		for (i = 0; i < pPlayer->GetCombatSkillPack()->GetElementCount(); i++)
			PCombatSkills->GetVisualData(1, i+2)->addAni(IDB_ENERGYBAR, "", 5, 1, 16, -1, 0);
		for (i = 0; i < pPlayer->GetSkillPack()->GetElementCount(); i++)
			PSkills->GetVisualData(1, i+2)->addAni(IDB_ENERGYBAR, "", 5, 1, 16, -1, 0);
		PMagicSkill->GetVisualData(1, 2)->addAni(IDB_ENERGYBAR, "", 5, 1, 16, -1, 0);
	}

	InitCloseButton(IDB_PANEL_CLOSE, "", 16, 16);
}

void CharInfo::restoreAni()
{
	int i;
	Humanized *pHumanized = dynamic_cast<Humanized*>(Focus);
	Player *pPlayer = dynamic_cast<Player*>(Focus);

	Panel::restoreAni();
	PSkills->restoreAni();
	PCombatSkills->restoreAni();
	PMagicSkill->restoreAni();
	PStats->restoreAni();
	PStats2->restoreAni();
	PMagicSchools->restoreAni();

	PStats->GetVisualData(0, 9)->restoreAni();
	for (i = 0; i < Focus->GetStatPack()->GetElementCount(); i++)
	{
		PStats->GetVisualData(1, i+2)->restoreAni();
		PStats->GetVisualData(2, i+2)->restoreAni();
	}

	if (pHumanized)
	{
		PMagicSchools->GetVisualData(0, 5)->restoreAni();
		for (i = 0; i < pHumanized->GetMagicSchoolPack()->GetElementCount(); i++)
		{
			PMagicSchools->GetVisualData(1, i+2)->restoreAni();
			PMagicSchools->GetVisualData(2, i+2)->restoreAni();
		}
	}

	if (pPlayer)
	{
		for (i = 0; i < Focus->GetCombatSkillPack()->GetElementCount(); i++)
			PCombatSkills->GetVisualData(1, i+2)->restoreAni();
		for (i = 0; i < pHumanized->GetSkillPack()->GetElementCount(); i++)
			PSkills->GetVisualData(1, i+2)->restoreAni();
		PMagicSkill->GetVisualData(1, 2)->restoreAni();
	}
}

void CharInfo::destroyAni()
{	
	int i;
	Humanized *pHumanized = dynamic_cast<Humanized*>(Focus);
	Player *pPlayer = dynamic_cast<Player*>(Focus);

	Panel::destroyAni();
	PSkills->destroyAni();
	PCombatSkills->destroyAni();
	PMagicSkill->destroyAni();
	PStats->destroyAni();
	PStats2->destroyAni();
	PMagicSchools->destroyAni();

	PStats->GetVisualData(0, 9)->destroyAni();
	for (i = 0; i < Focus->GetStatPack()->GetElementCount(); i++)
	{
		PStats->GetVisualData(1, i+2)->destroyAni();
		PStats->GetVisualData(2, i+2)->destroyAni();
	}

	if (pHumanized)
	{
		PMagicSchools->GetVisualData(0, 5)->destroyAni();
		for (i = 0; i < pHumanized->GetMagicSchoolPack()->GetElementCount(); i++)
		{
			PMagicSchools->GetVisualData(1, i+2)->destroyAni();
			PMagicSchools->GetVisualData(2, i+2)->destroyAni();
		}
	}

	if (pPlayer)
	{
		for (i = 0; i < Focus->GetCombatSkillPack()->GetElementCount(); i++)
			PCombatSkills->GetVisualData(1, i+2)->destroyAni();
		for (i = 0; i < pHumanized->GetSkillPack()->GetElementCount(); i++)
			PSkills->GetVisualData(1, i+2)->destroyAni();
		PMagicSkill->GetVisualData(1, 2)->destroyAni();
	}
}

void CharInfo::DestroyText()
{
	Panel::DestroyText();
	PSkills->DestroyText();
	PCombatSkills->DestroyText();
	PMagicSkill->DestroyText();
	PStats->DestroyText();
	PStats2->DestroyText();
	PMagicSchools->DestroyText();
}

void CharInfo::AddForbidden2(Item *pItem)
{
	Panel::AddForbidden2(pItem);
	PSkills->AddForbidden2(pItem);
	PCombatSkills->AddForbidden2(pItem);
	PMagicSkill->AddForbidden2(pItem);
	PStats->AddForbidden2(pItem);
	PStats2->AddForbidden2(pItem);
	PMagicSchools->AddForbidden2(pItem);
}

void CharInfo::RemoveForbidden2(Item *pItem)
{
	Panel::RemoveForbidden2(pItem);
	PSkills->RemoveForbidden2(pItem);
	PCombatSkills->RemoveForbidden2(pItem);
	PMagicSkill->RemoveForbidden2(pItem);
	PStats->RemoveForbidden2(pItem);
	PStats2->RemoveForbidden2(pItem);
	PMagicSchools->RemoveForbidden2(pItem);
}

bool CharInfo::Run(Item *Cursor)
{
	if (Focus) return Panel::Run(Cursor);

	return false;
}

bool CharInfo::Refresh()
{
	int x, y;
	Humanized *pHumanized = dynamic_cast<Humanized*>(Focus);
	Player *pPlayer = dynamic_cast<Player*>(Focus);

	if (!Focus) return false;

	// Skills
	if (pHumanized)
	{
		Screen->LoadText(PSkills->GetTextData(0, 0)->GetSprite(), NULL, PSkills->getName()->Get(), RGB(255, 255, 255), RGB(0, 0, 0));
		PSkills->GetTextData(0, 0)->GetSprite()->SetColorKey(RGB(255, 255, 255));
		Screen->LoadText(PSkills->GetTextData(0, 1)->GetSprite(), NULL, "Nom", RGB(255, 255, 255), RGB(0, 0, 0));
		PSkills->GetTextData(0, 1)->GetSprite()->SetColorKey(RGB(255, 255, 255));
		Screen->LoadText(PSkills->GetTextData(1, 1)->GetSprite(), NULL, "Gain", RGB(255, 255, 255), RGB(0, 0, 0));
		PSkills->GetTextData(1, 1)->GetSprite()->SetColorKey(RGB(255, 255, 255));
		Screen->LoadText(PSkills->GetTextData(2, 1)->GetSprite(), NULL, "Niveau", RGB(255, 255, 255), RGB(0, 0, 0));
		PSkills->GetTextData(2, 1)->GetSprite()->SetColorKey(RGB(255, 255, 255));
		for (y = 2; y < pHumanized->GetSkillPack()->GetElementCount() + 2; y++)
		{
			x = 0;
			Screen->LoadText(PSkills->GetTextData(x, y)->GetSprite(), NULL, pHumanized->GetSkillPack()->getCharSkillElement(y-2)->GetDescription(), RGB(255, 255, 255), RGB(0, 0, 0));
			PSkills->GetTextData(x, y)->GetSprite()->SetColorKey(RGB(255, 255, 255));
			x = 2;
			Screen->LoadText(PSkills->GetTextData(x, y)->GetSprite(), NULL, pHumanized->GetSkillPack()->getCharSkillElement(y-2)->GetLevel(), RGB(255, 255, 255), RGB(0, 0, 0));
			PSkills->GetTextData(x, y)->GetSprite()->SetColorKey(RGB(255, 255, 255));
		}
		PSkills->Refresh();
	}

	// Skills de combat
	Screen->LoadText(PCombatSkills->GetTextData(0, 0)->GetSprite(), NULL, PCombatSkills->getName()->Get(), RGB(255, 255, 255), RGB(0, 0, 0));
	PCombatSkills->GetTextData(0, 0)->GetSprite()->SetColorKey(RGB(255, 255, 255));
	Screen->LoadText(PCombatSkills->GetTextData(0, 1)->GetSprite(), NULL, "Nom", RGB(255, 255, 255), RGB(0, 0, 0));
	PCombatSkills->GetTextData(0, 1)->GetSprite()->SetColorKey(RGB(255, 255, 255));
	Screen->LoadText(PCombatSkills->GetTextData(1, 1)->GetSprite(), NULL, "Gain", RGB(255, 255, 255), RGB(0, 0, 0));
	PCombatSkills->GetTextData(1, 1)->GetSprite()->SetColorKey(RGB(255, 255, 255));
	Screen->LoadText(PCombatSkills->GetTextData(2, 1)->GetSprite(), NULL, "Niveau", RGB(255, 255, 255), RGB(0, 0, 0));
	PCombatSkills->GetTextData(2, 1)->GetSprite()->SetColorKey(RGB(255, 255, 255));
	for (y = 2; y < Focus->GetCombatSkillPack()->GetElementCount() + 2; y++)
	{
		x = 0;
		Screen->LoadText(PCombatSkills->GetTextData(x, y)->GetSprite(), NULL, Focus->GetCombatSkillPack()->getCharSkillElement(y-2)->GetDescription(), RGB(255, 255, 255), RGB(0, 0, 0));
		PCombatSkills->GetTextData(x, y)->GetSprite()->SetColorKey(RGB(255, 255, 255));
		x = 2;
		Screen->LoadText(PCombatSkills->GetTextData(x, y)->GetSprite(), NULL, Focus->GetCombatSkillPack()->getCharSkillElement(y-2)->GetLevel(), RGB(255, 255, 255), RGB(0, 0, 0));
		PCombatSkills->GetTextData(x, y)->GetSprite()->SetColorKey(RGB(255, 255, 255));
	}
	PCombatSkills->Refresh();

	// Skill de magie
	if (pHumanized)
	{
		Screen->LoadText(PMagicSkill->GetTextData(0, 0)->GetSprite(), NULL, PMagicSkill->getName()->Get(), RGB(255, 255, 255), RGB(0, 0, 0));
		PMagicSkill->GetTextData(0, 0)->GetSprite()->SetColorKey(RGB(255, 255, 255));
		Screen->LoadText(PMagicSkill->GetTextData(0, 1)->GetSprite(), NULL, "Nom", RGB(255, 255, 255), RGB(0, 0, 0));
		PMagicSkill->GetTextData(0, 1)->GetSprite()->SetColorKey(RGB(255, 255, 255));
		Screen->LoadText(PMagicSkill->GetTextData(1, 1)->GetSprite(), NULL, "Gain", RGB(255, 255, 255), RGB(0, 0, 0));
		PMagicSkill->GetTextData(1, 1)->GetSprite()->SetColorKey(RGB(255, 255, 255));
		Screen->LoadText(PMagicSkill->GetTextData(2, 1)->GetSprite(), NULL, "Niveau", RGB(255, 255, 255), RGB(0, 0, 0));
		PMagicSkill->GetTextData(2, 1)->GetSprite()->SetColorKey(RGB(255, 255, 255));
		Screen->LoadText(PMagicSkill->GetTextData(0, 2)->GetSprite(), NULL, "Magie", RGB(255, 255, 255), RGB(0, 0, 0));
		PMagicSkill->GetTextData(0, 2)->GetSprite()->SetColorKey(RGB(255, 255, 255));
		Screen->LoadText(PMagicSkill->GetTextData(2, 2)->GetSprite(), NULL, pHumanized->GetMagicSkill()->GetLevel(), RGB(255, 255, 255), RGB(0, 0, 0));
		PMagicSkill->GetTextData(2, 2)->GetSprite()->SetColorKey(RGB(255, 255, 255));
		PMagicSkill->Refresh();
	}

	// Caractéristiques
	PStats->Refresh();

	// Caractéristiques secondaires
	Screen->LoadText(PStats2->GetTextData(0, 0)->GetSprite(), NULL, PStats2->getName()->Get(), RGB(255, 255, 255), RGB(0, 0, 0));
	PStats2->GetTextData(0, 0)->GetSprite()->SetColorKey(RGB(255, 255, 255));
	Screen->LoadText(PStats2->GetTextData(0, 1)->GetSprite(), NULL, "Nom", RGB(255, 255, 255), RGB(0, 0, 0));
	PStats2->GetTextData(0, 1)->GetSprite()->SetColorKey(RGB(255, 255, 255));
	Screen->LoadText(PStats2->GetTextData(1, 1)->GetSprite(), NULL, "Score", RGB(255, 255, 255), RGB(0, 0, 0));
	PStats2->GetTextData(1, 1)->GetSprite()->SetColorKey(RGB(255, 255, 255));
	for (y = 2; y < Focus->GetStat2Pack()->GetElementCount() + 2; y++)
	{
		x = 0;
		Screen->LoadText(PStats2->GetTextData(x, y)->GetSprite(), NULL, Focus->GetStat2Pack()->getCharStatElement(y-2)->GetDescription(), RGB(255, 255, 255), RGB(0, 0, 0));
		PStats2->GetTextData(x, y)->GetSprite()->SetColorKey(RGB(255, 255, 255));
		x = 1;
		Screen->LoadText(PStats2->GetTextData(x, y)->GetSprite(), NULL, Focus->GetStat2Pack()->getCharStatElement(y-2)->GetScore(), RGB(255, 255, 255), RGB(0, 0, 0));
		PStats2->GetTextData(x, y)->GetSprite()->SetColorKey(RGB(255, 255, 255));
	}
	PStats2->Refresh();

	// Ecoles de magie
	PMagicSchools->Refresh();

	Screen->LoadText(((Text*)Grid[0][0].Content)->GetSprite(), NULL, name, RGB(255, 255, 255), RGB(0, 0, 0));
	((Text*)Grid[0][0].Content)->GetSprite()->SetColorKey(RGB(255, 255, 255));

	return Panel::Refresh();
}

bool CharInfo::generate(bool waited, Cursor *cursor)
{
	PStats->generate(waited, cursor);
	PMagicSchools->generate(waited, cursor);

	return Panel::generate(waited, cursor);
}
