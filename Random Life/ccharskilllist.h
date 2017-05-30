#ifndef CCHARSKILLLIST
#define CCHARSKILLLIST

#include <clist.h>
#include "ccharskill.h"

class CharSkillList : public List
{
private:
public:
	CharSkillList() {}
	CharSkillList(int List_Order) : List(List_Order) {}
	~CharSkillList() {}
	int SearchElementBySkillId(int SkillId);
	CharSkill* GetSkill(int SkillId);
	CharSkill* GetBestSkill();
	CharSkill* GetBestArmorSkill();
	CharSkill* GetBestCombatSkill(bool HeavyOnly=false);
	CharSkill* getCharSkillElement(int index) {return dynamic_cast<CharSkill*>(GetElement(index));}
};


#endif