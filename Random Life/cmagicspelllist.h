#ifndef CMAGICSPELLLIST
#define CMAGICSPELLLIST

#include <clist.h>
#include "cmagicspell.h"

class MagicSpellList : public List
{
private:
public:
	static const int ORDER_BY_LEVEL = 10;
	MagicSpellList() {}
	MagicSpellList(int List_Order) : List(List_Order) {}
	~MagicSpellList() {}
	int OrderBy(int NewOrder, Container *pContainer);
	MagicSpell* GetHigherMagicSpell(int effect) {return GetHigherMagicSpell(MagicSpell::magicSpellSchool(effect), effect, 0);}
	MagicSpell* GetHigherMagicSpell(int school, int effect, int SkipEffect);
	MagicSpell* getMagicSpellElement(int index) {return dynamic_cast<MagicSpell*>(GetElement(index));}
};

#endif