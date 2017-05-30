#include "cmagicspelllist.h"

int MagicSpellList::OrderBy(int NewOrder, Container *pContainer)
{
	int i, result;

	result = List::OrderBy(NewOrder, pContainer);

	if (result == ORDER_UNKNOWN)
	{
		switch (NewOrder)
		{
		case ORDER_BY_LEVEL:
			for (i = 0; i < ElementCount; i++)
				pList[i].Content->getSortData()->num = getMagicSpellElement(i)->GetLevel();
			result = OrderElements(ORDER_NUM);
			if (!result) return result;
			break;
		default:
			return ORDER_UNKNOWN;
		}
	}
	else
		return result;

	FinalizeOrder(NewOrder);

	return result;
}

MagicSpell * MagicSpellList::GetHigherMagicSpell(int school, int effect, int SkipEffect)
{
	int i;
	MagicSpellList SpellPack(ORDER_BY_LEVEL);

	for (i = 0; i < ElementCount; i++)
		if (getMagicSpellElement(i)->GetSchool() == school)
			if ((getMagicSpellElement(i)->GetEffect() == effect) | (effect == 0))
				if ((getMagicSpellElement(i)->GetEffect() != SkipEffect) | (SkipEffect == 0))
					SpellPack.AddElement(getMagicSpellElement(i));

	if (SpellPack.GetElementCount())
		return SpellPack.getMagicSpellElement(SpellPack.GetElementCount()-1);

	return NULL;
}
