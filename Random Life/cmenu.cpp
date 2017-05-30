#include "cmenu.h"
#include "cdataprogress.h"
#include "../RLRes/RLRes/resource.h"

extern Display *Screen;
extern DataProgress *PData;
extern void NewGame();
extern bool CanLoadData();

Menu::Menu()
{
	int i;

	ButtonsContainer = new Panel();

	for (i = 0; i < ButtonsContainer->GetRowCount(); i++)
		Button[i] = new Panel();
}

Menu::Menu(int Panel_ColSpace, int Panel_RowSpace, int Panel_BorderSpace) :
Panel(1, 3, Panel_ColSpace, Panel_RowSpace, Panel_BorderSpace, false, Text(), NULL, NULL, Str())
{
	IntroText = (Str)"Bienvenue dans les Terres. Vous débarquez dans ce petit village qui autrefois bien paisible, paraît aujourd'hui en pleine désolation. En effet, une terrible malédiction rendant les gens fous, agressifs et cléptomanes, s'y-est abattue. " +
		(Str)"Quelle en en est la cause? Discutez avec les gens du Village pour obtenir des explications et ce que vous pouvez faire. Le Village a besoin d'un sauveur! A vous de décider si vous vous sentez l'âme d'un Héro. " +
		(Str)"Sachez que la Malédiction est propagée sur l'entier des Terres. Seuls les habitants du Village lui résiste encore. Mais bientôt le fléau aura raison d'eux. Alors sera venu le temps du Chao...";

	ButtonsContainer = new Panel(1, 3, 0, 10, 0, false, Text(), 128, 128, Str());

	Button[0] = new Panel(1, 1, 0, 0, 6, false, Text(), 160, 32, "Nouvelle partie");
	Button[1] = new Panel(1, 1, 0, 0, 6, false, Text(), 160, 32, "Charger partie");
	Button[2] = new Panel(1, 1, 0, 0, 6, false, Text(), 160, 32, "Quitter");
}

Menu::~Menu()
{
	int i;

	for (i = 0; i < ButtonsContainer->GetRowCount(); i++)
		delete Button[i];

	delete ButtonsContainer;
}

void Menu::Init()
{
	int i;

	Grid[0][0].data = VISUAL;
	Grid[1][0].data = VISUAL_INHERIT;

	for (i = 0; i < RowCount; i++)
	{
		Grid[i][0].halign = HALIGN_CENTER;
		AlignColsOnRow(i);
	}

	for (i = 0; i < ButtonsContainer->GetRowCount(); i++)
	{
		Button[i]->GetGrid(0, 0)->ColSpan = -1;
		Button[i]->GetGrid(0, 0)->halign = HALIGN_CENTER;
		ButtonsContainer->GetGrid(0, i)->data = VISUAL_INHERIT;
	}

	Panel::Init();
	ButtonsContainer->Init();

	for (i = 0; i < ButtonsContainer->GetRowCount(); i++)
	{
		Button[i]->Init();
		ButtonsContainer->GetGrid(0, i)->Content = Button[i];
	}

	Grid[1][0].Content = ButtonsContainer;
}

void Menu::initAni()
{
	int i;

	GetVisualData(0, 0)->addAni(IDB_TITLE, "", 1, 435, 70, RGB(255, 255, 255), 270);
	InitBackground(IDB_MENU_BKG, "", 16, 16);

	for (i = 0; i < ButtonsContainer->GetRowCount(); i++)
	{
		Button[i]->InitBackground(IDB_PANEL_BKG, "", 16, 16);
		Button[i]->initBorders(IDB_PANEL_BORDER, "", 61, 6);
	}
}

void Menu::Initialize()
{
	int i;

	Panel::Initialize();
	ButtonsContainer->Initialize();

	for (i = 0; i < ButtonsContainer->GetRowCount(); i++)
		Button[i]->Initialize();
}

void Menu::restoreAni()
{
	int i;

	Panel::restoreAni();
	GetVisualData(0, 0)->restoreAni();

	ButtonsContainer->restoreAni();

	for (i = 0; i < ButtonsContainer->GetRowCount(); i++)
		Button[i]->restoreAni();
}

void Menu::destroyAni()
{
	int i;

	Panel::destroyAni();
	GetVisualData(0, 0)->destroyAni();

	ButtonsContainer->destroyAni();

	for (i = 0; i < ButtonsContainer->GetRowCount(); i++)
		Button[i]->destroyAni();
}

void Menu::DestroyText()
{
	int i;

	Panel::DestroyText();
	ButtonsContainer->DestroyText();

	for (i = 0; i < ButtonsContainer->GetRowCount(); i++)
		Button[i]->DestroyText();
}

bool Menu::Refresh()
{
	int i;

	for (i = 0; i < ButtonsContainer->GetRowCount(); i++)
	{
		Screen->LoadText(Button[i]->GetTextData(0, 0)->GetSprite(), NULL, Button[i]->getName()->Get(), RGB(255, 255, 255), RGB(0, 0, 0));
		Button[i]->GetTextData(0, 0)->GetSprite()->SetColorKey(RGB(255, 255, 255));
		Button[i]->Refresh();
	}

	if (CanLoadData())
		ButtonsContainer->GetGrid(0, 1)->display = true;
	else
		ButtonsContainer->GetGrid(0, 1)->display = false;

	ButtonsContainer->Refresh();

	((Text*)Grid[2][0].Content)->Update(dimension.x - (BorderSpace * 2), RGB(0, 0, 0), RGB(255, 255, 255), &IntroText);

	return Panel::Refresh();
}

bool Menu::generate(bool waited, Cursor *cursor)
{
	if (ButtonsContainer->ClickOnGrid(0, 0, waited, cursor))
		NewGame();

	if (ButtonsContainer->ClickOnGrid(0, 1, waited, cursor))
		PData->CreateLoadThread();

	if (ButtonsContainer->ClickOnGrid(0, 2, waited, cursor))
	{
		SendMessage(Screen->GetDxWnd(), WM_QUIT, 1, NULL);
		return false;
	}

	ButtonsContainer->generate(waited, cursor);

	return Panel::generate(waited, cursor);
}
