#include "ccharskilllist.h"

int CharSkillList::SearchElementBySkillId(int SkillId)
{
	int i;

	for (i = 0; i < ElementCount; i++)
		if (getCharSkillElement(i)->GetSkillId() == SkillId)
			return i;

	return -1;
}

CharSkill * CharSkillList::GetSkill(int SkillId)
{
	int i;

	for (i = 0; i < ElementCount; i++)
		if (getCharSkillElement(i)->GetSkillId() == SkillId)
			return getCharSkillElement(i);

	return NULL;
}

CharSkill * CharSkillList::GetBestSkill()
{
	int i;
	CharSkill *BestSkill = GetSkill(MEDITATION);

	for (i = 0; i < ElementCount; i++)
		if (getCharSkillElement(i)->GetLevel() >= BestSkill->GetLevel())
			BestSkill = getCharSkillElement(i);

	return BestSkill;
}

CharSkill * CharSkillList::GetBestArmorSkill()
{
	CharSkill *LightArmorSkill = GetSkill(LIGHT_ARMOR);
	CharSkill *HeavyArmorSkill = GetSkill(HEAVY_ARMOR);

	if (HeavyArmorSkill->GetLevel() > LightArmorSkill->GetLevel())
		return HeavyArmorSkill;

	return LightArmorSkill;
}

CharSkill * CharSkillList::GetBestCombatSkill(bool HeavyOnly)
{
	int i;
	CharSkill *BestCombatSkill;

	if (HeavyOnly)
		BestCombatSkill = GetSkill(MACES);
	else
		BestCombatSkill = GetSkill(HAND_TO_HAND);

	for (i = 0; i < ElementCount; i++)
		if (getCharSkillElement(i)->GetLevel() >= BestCombatSkill->GetLevel())
			if (((getCharSkillElement(i)->GetSkillId() == SWORDS) | (getCharSkillElement(i)->GetSkillId() == AXES)) | (HeavyOnly == false))
				BestCombatSkill = getCharSkillElement(i);

	return BestCombatSkill;
}
