#include "cmagiclearner.h"
#include "../RLRes/RLRes/resource.h"

extern Display *Screen;

MagicLearner::MagicLearner()
{
	Focus = NULL;
	Cure = new SpellList();
	Destruction = new SpellList();
	Protection = new SpellList();
}

MagicLearner::MagicLearner(int Panel_ColSpace, int Panel_RowSpace, int Panel_BorderSpace) :
Panel(3, 1, Panel_ColSpace, Panel_RowSpace, Panel_BorderSpace, false, Text(), Str()),
Item(NULL, NULL, NULL, NULL, "MAGICLEARNER")
{
	Focus = NULL;
	Cure = new SpellList(true, false, CURE, false, 6, 5, 2, 6, false, 64, 192, "GUER");
	Destruction = new SpellList(true, false, DESTRUCTION, true, 6, 5, 2, 6, false, 64, 192, "DEST");
	Protection = new SpellList(true, false, PROTECTION, true, 6, 5, 2, 6, false, 64, 192, "PROT");
}

MagicLearner::~MagicLearner()
{
	delete Cure;
	delete Destruction;
	delete Protection;
}

void MagicLearner::SetFocus(Humanized *ML_Focus)
{
	Focus = ML_Focus;

	Cure->SetMagician(Focus);
	Destruction->SetMagician(Focus);
	Protection->SetMagician(Focus);
}

void MagicLearner::Init()
{
	int i, j;

	for (i = 0; i < ColCount; i++)
		for (j = 0; j < RowCount; j++)
			Grid[j][i].data = VISUAL_INHERIT;

	Panel::Init();
	Cure->Init();
	Destruction->Init();
	Protection->Init();

	Grid[0][0].Content = Cure;
	Grid[0][1].Content = Destruction;
	Grid[0][2].Content = Protection;
}

void MagicLearner::Initialize()
{
	Panel::Initialize();
	Cure->Initialize();
	Destruction->Initialize();
	Protection->Initialize();
}

void MagicLearner::initAni()
{
	Cure->initAni();
	Destruction->initAni();
	Protection->initAni();

	InitCloseButton(IDB_PANEL_CLOSE, "", 16, 16);
}

void MagicLearner::restoreAni()
{
	Panel::restoreAni();
	Cure->restoreAni();
	Destruction->restoreAni();
	Protection->restoreAni();
}

void MagicLearner::destroyAni()
{
	Panel::destroyAni();
	Cure->destroyAni();
	Destruction->destroyAni();
	Protection->destroyAni();
}

void MagicLearner::DestroyText()
{
	Panel::DestroyText();
	Cure->DestroyText();
	Destruction->DestroyText();
	Protection->DestroyText();
}

void MagicLearner::AddForbidden2(Item *pItem)
{
	Panel::AddForbidden2(pItem);
	Cure->AddForbidden2(pItem);
	Destruction->AddForbidden2(pItem);
	Protection->AddForbidden2(pItem);
}

void MagicLearner::RemoveForbidden2(Item *pItem)
{
	Panel::RemoveForbidden2(pItem);
	Cure->RemoveForbidden2(pItem);
	Destruction->RemoveForbidden2(pItem);
	Protection->RemoveForbidden2(pItem);
}

bool MagicLearner::Run(Item *Cursor)
{
	if (Focus) return Panel::Run(Cursor);

	return false;
}

bool MagicLearner::Refresh()
{
	Cure->Refresh();
	Destruction->Refresh();
	Protection->Refresh();

	return Panel::Refresh();
}

bool MagicLearner::generate(bool waited, Cursor *cursor)
{	
	Cure->generate(waited, cursor);
	Destruction->generate(waited, cursor);
	Protection->generate(waited, cursor);

	if (Destruction->GetSelector()->Slide(cursor))
		Destruction->Refresh();
	if (Protection->GetSelector()->Slide(cursor))
		Protection->Refresh();

	return Panel::generate(waited, cursor);
}

bool MagicLearner::CanMove(Item *pItem)
{
	if (!Cure->CanMove(pItem)) return false;
	if (!Destruction->CanMove(pItem)) return false;
	if (!Protection->CanMove(pItem)) return false;

	return Panel::CanMove(pItem);
}

bool MagicLearner::CanClick(Item *pItem)
{
	if (!Cure->CanClick(pItem)) return false;
	if (!Destruction->CanClick(pItem)) return false;
	if (!Protection->CanClick(pItem)) return false;

	return Panel::CanClick(pItem);
}
