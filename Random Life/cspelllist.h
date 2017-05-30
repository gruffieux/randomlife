#ifndef CSPELLLIST
#define CSPELLLIST

#include "ciconlist.h"
#include "cmagicspelllist.h"
#include "chumanized.h"
#include "main.h"

class SpellList : public IconList
{
private:
	bool learner, HigherOnly;
	CHAR_MAGIC_SCHOOL school;
	MagicSpellList AllSpells;
	Humanized *Magician;
	void InitAllSpells();
public:
	SpellList();
	SpellList(bool SL_learner, bool SL_HigherOnly, CHAR_MAGIC_SCHOOL SL_school, bool IL_UseSelector, int Panel_RowCount, int Panel_ColSpace, int Panel_RowSpace, int Panel_BorderSpace, bool Panel_save, Str Object_Name);
	SpellList(bool SL_learner, bool SL_HigherOnly, CHAR_MAGIC_SCHOOL SL_school, bool IL_UseSelector, int Panel_RowCount, int Panel_ColSpace, int Panel_RowSpace, int Panel_BorderSpace, bool Panel_save, int Item_width, int Item_height, Str Object_Name);
	~SpellList() {;}
	void SetMagician(Humanized *SL_Magician) {Magician = SL_Magician;}
	void initAni();
	bool Run(Item *Cursor);
	bool Refresh();
	bool generate(bool waited=false, Cursor *cursor=NULL);
};

#endif