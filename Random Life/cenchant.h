#ifndef CENCHANT
#define CENCHANT

#include <cobject.h>
#include <clist.h>
#include "main.h"

class Enchant : public Object
{
private:
	int MinValue, MaxValue;
	PICKUP_ENCHANT effect;
public:
	Enchant();
	Enchant(int PE_MinValue, int PE_MaxValue, PICKUP_ENCHANT PE_effect);
	Str GetDescription();
	int GetMinValue() {return MinValue;}
	int GetMaxValue() {return MaxValue;}
	PICKUP_ENCHANT GetEffect() {return effect;}
	static Enchant* getEnchantElement(int index, List *pList) {return dynamic_cast<Enchant*>(pList->GetElement(index));}
};

#endif