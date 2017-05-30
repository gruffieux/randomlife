#include "caskforsave.h"
#include "cdataprogress.h"
#include "../RLRes/RLRes/resource.h"

extern Display *Screen;
extern DataProgress *PData;

AskForSave::AskForSave(int Panel_ColSpace, int Panel_RowSpace, int Panel_BorderSpace) :
Panel(1, 4, Panel_ColSpace, Panel_RowSpace, Panel_BorderSpace, false, Text(), NULL, NULL, Str()),
Item(NULL, NULL, 240, 96, "SAUVER LA PARTIE?")
{
	Choices = new Panel(4, 1, 5, 2, 5, false, Text(), Str());
}

void AskForSave::SetRemainingPoints(int AFS_SkillPoints, int AFS_MagicPoints)
{
	SkillPoints = AFS_SkillPoints;
	MagicPoints = AFS_MagicPoints;
}

void AskForSave::Init()
{
	int i;

	Grid[3][0].data = VISUAL_INHERIT;
	Choices->GetGrid(1, 0)->data = Choices->GetGrid(3, 0)->data = VISUAL;

	for (i = 0; i < RowCount; i++)
	{
		Grid[i][0].ColSpan = -1;
		Grid[i][0].halign = HALIGN_CENTER;
	}

	Panel::Init();
	Choices->Init();

	Grid[3][0].Content = Choices;

	Choices->AddForbidden1(Choices->GetVisualData(1, 0));
	Choices->AddForbidden1(Choices->GetVisualData(3, 0));
}

void AskForSave::Initialize()
{
	Choices->Initialize();
	Panel::Initialize();
}

void AskForSave::initAni()
{
	Choices->GetVisualData(1, 0)->addAni(IDB_SELECT_BUTTONS, "", 1, 16, 16, -1, 90);
	Choices->GetVisualData(3, 0)->addAni(IDB_SELECT_BUTTONS, "", 1, 16, 16, -1, 90);
}

void AskForSave::restoreAni()
{
	Panel::restoreAni();
	
	Choices->GetVisualData(1, 0)->restoreAni();
	Choices->GetVisualData(3, 0)->restoreAni();
}

void AskForSave::destroyAni()
{
	Panel::destroyAni();
	
	Choices->GetVisualData(1, 0)->destroyAni();
	Choices->GetVisualData(3, 0)->destroyAni();
}

void AskForSave::DestroyText()
{
	Panel::DestroyText();
	Choices->DestroyText();
}

bool AskForSave::Refresh()
{
	Screen->LoadText(Choices->GetTextData(0, 0)->GetSprite(), NULL, "Oui", RGB(255, 255, 255), RGB(0, 0, 0));
	Choices->GetTextData(0, 0)->GetSprite()->SetColorKey(RGB(255, 255, 255));
	Screen->LoadText(Choices->GetTextData(2, 0)->GetSprite(), NULL, "Non", RGB(255, 255, 255), RGB(0, 0, 0));
	Choices->GetTextData(2, 0)->GetSprite()->SetColorKey(RGB(255, 255, 255));
	
	Choices->Refresh();

	Screen->LoadText(((Text*)Grid[0][0].Content)->GetSprite(), NULL, name, RGB(255, 255, 255), RGB(0, 0, 0));
	((Text*)Grid[0][0].Content)->GetSprite()->SetColorKey(RGB(255, 255, 255));
	Screen->LoadText(((Text*)Grid[1][0].Content)->GetSprite(), NULL, BuildString("SkillPoints restants: %d", SkillPoints), RGB(255, 255, 255), RGB(0, 0, 0));
	((Text*)Grid[1][0].Content)->GetSprite()->SetColorKey(RGB(255, 255, 255));
	Screen->LoadText(((Text*)Grid[2][0].Content)->GetSprite(), NULL, BuildString("MagicPoints restants: %d", MagicPoints), RGB(255, 255, 255), RGB(0, 0, 0));
	((Text*)Grid[2][0].Content)->GetSprite()->SetColorKey(RGB(255, 255, 255));

	return Panel::Refresh();
}

bool AskForSave::generate(bool waited, Cursor *cursor)
{
	if (Choices->ClickOnGrid(1, 0, waited, cursor))
	{
		hide();
		PData->show();
		PData->Refresh();
		PData->CreateSaveThread();
	}

	if (Choices->ClickOnGrid(3, 0, waited, cursor))
		hide();
	
	return Panel::generate(waited, cursor);
}

bool AskForSave::CanMove(Item *pItem)
{
	if (!Choices->CanMove(pItem)) return false;

	return Panel::CanMove(pItem);
}

bool AskForSave::CanClick(Item *pItem)
{
	if (!Choices->CanClick(pItem)) return false;

	return Panel::CanClick(pItem);
}
