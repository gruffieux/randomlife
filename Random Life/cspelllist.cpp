#include "cspelllist.h"
#include "clookedspell.h"
#include "../RLRes/RLRes/resource.h"

extern Display *Screen;
extern LookedSpell *PMagicSpell;

SpellList::SpellList()
{
	learner = HigherOnly = false;
	school = NONE_CHAR_MAGIC_SCHOOL;
	Magician = NULL;
}

SpellList::SpellList(bool SL_learner, bool SL_HigherOnly, CHAR_MAGIC_SCHOOL SL_school, bool IL_UseSelector, int Panel_RowCount, int Panel_ColSpace, int Panel_RowSpace, int Panel_BorderSpace, bool Panel_save, Str Object_Name) :
IconList(IL_UseSelector, Panel_RowCount, Panel_ColSpace, Panel_RowSpace, Panel_BorderSpace, Panel_save, Str()),
Item(NULL, NULL, NULL, NULL, Object_Name)
{
	learner = SL_learner;
	HigherOnly = SL_HigherOnly;
	school = SL_school;
	Magician = NULL;
}

SpellList::SpellList(bool SL_learner, bool SL_HigherOnly, CHAR_MAGIC_SCHOOL SL_school, bool IL_UseSelector, int Panel_RowCount, int Panel_ColSpace, int Panel_RowSpace, int Panel_BorderSpace, bool Panel_save, int Item_width, int Item_height, Str Object_Name) :
IconList(IL_UseSelector, Panel_RowCount, Panel_ColSpace, Panel_RowSpace, Panel_BorderSpace, Panel_save, Item_width, Item_height, Str()),
Item(NULL, NULL, Item_width, Item_height, Object_Name)
{
	learner = SL_learner;
	HigherOnly = SL_HigherOnly;
	school = SL_school;
	Magician = NULL;
}

void SpellList::InitAllSpells()
{
	int i, j;
	MagicSpell *pMagicSpell;

	AllSpells.RemoveAllElement();

	for (i = 0; i < Magician->GetMagicSchoolPack()->GetElementCount(); i++)
		if (HigherOnly)
		{
			for (j = 1; j < TOTAL_CHAR_MAGIC_SPELL_EFFECT; j++)
			{
				pMagicSpell = Magician->GetMagicSpellPack(i)->GetHigherMagicSpell(j);
				if (pMagicSpell)
					AllSpells.AddElement(pMagicSpell);
			}
		}
		else
			for (j = 0; j < Magician->GetMagicSpellPack(i)->GetElementCount(); j++)
				AllSpells.AddElement(Magician->GetMagicSpellPack(i)->getMagicSpellElement(j));
}

void SpellList::initAni()
{
	IconList::initAni();
	InitIcons(IDB_SPELLLIST, "", 7, 32, 32);
}

bool SpellList::Run(Item *Cursor)
{
	if (Magician) return Panel::Run(Cursor);

	return false;
}

bool SpellList::Refresh()
{
	int i, SpellID;

	if (!Magician) return false;

	// Si on ne prend pas en compte l'école, on dresse la liste de tous les sorts dispo
	if (school == NONE_CHAR_MAGIC_SCHOOL)
		InitAllSpells();

	// Affichage des icônes pour chaque sort
	for (i = 1; i < RowCount; i++)
		if (Selector)
		{
			if (school)
			{
				SpellID = i - 1 + Selector->GetStage(Magician->GetMagicSpellPack(school-1)->GetElementCount()-1);
				if (SpellID < Magician->GetMagicSpellPack(school-1)->GetElementCount())
				{
					if (Magician->GetMagicSpellPack(school-1)->ElementExist(SpellID))
						Grid[i][1].PicFrame = Magician->GetMagicSpellPack(school-1)->getMagicSpellElement(SpellID)->GetEffect();
				}
				else
					Grid[i][1].PicFrame = 0;
			}
			else
			{
				SpellID = i - 1 + Selector->GetStage(AllSpells.GetElementCount()-1);
				if (SpellID < AllSpells.GetElementCount())
				{
					if (AllSpells.ElementExist(SpellID))
						Grid[i][1].PicFrame = AllSpells.getMagicSpellElement(SpellID)->GetEffect();
				}
				else
					Grid[i][1].PicFrame = 0;
			}
		}
		else
			if (school)
			{
				if (Magician->GetMagicSpellPack(school-1)->ElementExist(i-1))
					Grid[i][1].PicFrame = Magician->GetMagicSpellPack(school-1)->getMagicSpellElement(i-1)->GetEffect();
				else
					Grid[i][1].PicFrame = 0;
			}
			else
				if (AllSpells.ElementExist(i-1))
					Grid[i][1].PicFrame = AllSpells.getMagicSpellElement(i-1)->GetEffect();
				else
					Grid[i][1].PicFrame = 0;

	return IconList::Refresh();
}

bool SpellList::generate(bool waited, Cursor *cursor)
{	
	int i, SpellID;

	if (!Magician) return false;

	if (Selector && Selector->Slide(cursor))
		Refresh();

	// Evénements sur chaque sort
	for (i = 1; i < RowCount; i++)
		if (ClickOnGrid(1, i, waited, cursor))
		{
			PMagicSpell->SetLearn(learner);
			if (Selector)
			{
				if (school)
				{
					SpellID = i - 1 + Selector->GetStage(Magician->GetMagicSpellPack(school-1)->GetElementCount()-1);
					if (SpellID < Magician->GetMagicSpellPack(school-1)->GetElementCount())
						PMagicSpell->SetFocus(Magician->GetMagicSpellPack(school-1)->getMagicSpellElement(SpellID));
				}
				else
				{
					SpellID = i - 1 + Selector->GetStage(AllSpells.GetElementCount()-1);
					if (SpellID < AllSpells.GetElementCount())
						PMagicSpell->SetFocus(AllSpells.getMagicSpellElement(SpellID));
				}
			}
			else
				if (school)
				{
					if (Magician->GetMagicSpellPack(school-1)->ElementExist(i-1))
						PMagicSpell->SetFocus(Magician->GetMagicSpellPack(school-1)->getMagicSpellElement(i-1));
				}
				else
					if (AllSpells.ElementExist(i-1))
						PMagicSpell->SetFocus(AllSpells.getMagicSpellElement(i-1));

			PMagicSpell->show();
			PMagicSpell->Refresh();
		}

		return IconList::generate(waited, cursor);
}
