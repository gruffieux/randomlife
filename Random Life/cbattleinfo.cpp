#include "cbattleinfo.h"
#include "ccombatsystemlist.h"

extern Display *Screen;
extern CombatSystemList BattlePack;

BattleInfo::BattleInfo(int Panel_ColSpace, int Panel_RowSpace, int Panel_BorderSpace) :
Panel(1, 3, Panel_ColSpace, Panel_RowSpace, Panel_BorderSpace, true, Text(), Str()),
Item(NULL, NULL, NULL, NULL, "COMBAT!")
{
}

void BattleInfo::Init()
{
	Grid[0][0].ColSpan = -1;
	Grid[0][0].halign = HALIGN_CENTER;

	Panel::Init();
}

bool BattleInfo::Refresh()
{
	int i;

	i = BattlePack.SearchElementWithPanel();

	if (i == -1) return false;
	if (!BattlePack.GetCombatSystemElement(i)->GetRunning()) return false;
	if (!BattlePack.GetCombatSystemElement(i)->GetCombatantPack()->GetElementCount()) return false;

	Screen->LoadText(((Text*)Grid[0][0].Content)->GetSprite(), NULL, BattlePack.GetCombatSystemElement(i)->GetPTurn()->getName()->Get(), RGB(255, 255, 255), RGB(0, 0, 0));
	((Text*)Grid[0][0].Content)->GetSprite()->SetColorKey(RGB(255, 255, 255));
	Screen->LoadText(((Text*)Grid[1][0].Content)->GetSprite(), NULL, (Str)"Tour de " + Character::getCharacterElement(BattlePack.GetCombatSystemElement(i)->GetTurn(), BattlePack.GetCombatSystemElement(i)->GetCombatantPack())->GetDisplayedName()->Get(), RGB(255, 255, 255), RGB(0, 0, 0));
	((Text*)Grid[1][0].Content)->GetSprite()->SetColorKey(RGB(255, 255, 255));
	Screen->LoadText(((Text*)Grid[2][0].Content)->GetSprite(), NULL, (Str)"Phase " + BattlePack.GetCombatSystemElement(i)->GetPhase(), RGB(255, 255, 255), RGB(0, 0, 0));
	((Text*)Grid[2][0].Content)->GetSprite()->SetColorKey(RGB(255, 255, 255));

	return Panel::Refresh();
}
