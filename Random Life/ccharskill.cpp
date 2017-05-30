#include "ccharskill.h"

CharSkill::CharSkill()
{
	SkillId = level = difficulty = SRS = SuccessCount = 0;
	xp = 0;
	used = false;
	result = NONE_CHAR_SKILL_RESULT;
}

CharSkill::CharSkill(CharSkill &model)
{
	SkillId = model.GetSkillId();
	level = model.GetLevel();
	difficulty = model.GetDifficulty();
	SRS = model.GetSRS();
	xp = model.GetXP();
	SuccessCount = model.GetSuccessCount();
	used = model.GetUsed();
	result = model.GetResult();
	BPData = *model.GetBPData();
	Description = model.GetDescription();
	StatPack = *model.GetStatPack();
}

void CharSkill::InitXP()
{
	SuccessCount = 0;
	xp = 0;
}

void CharSkill::GenerateResult(ORC ORCData[])
{
	int i, j, DiceScore;

	DiceScore = Clock::random(3, 18);
	result = NONE_CHAR_SKILL_RESULT;

	//Définit le résultat de la skill en comparant le score des dés à la ORC.
	//Ref: "The Freestyle RPG Basic Rules", page 16.
	for (i = 1; i < TOTAL_CHAR_SKILL_RESULT; i++)
	{
		for (j = 0; j < TOTAL_ORC_RECORD; j++)
		{
			if ((SRS <= -10) && (DiceScore >= ORCData[i].min[1]) && (DiceScore <= ORCData[i].max[1]))
				result = (CHAR_SKILL_RESULT)i;
			if ((SRS == ORCData[i].SRS[j]) && (DiceScore >= ORCData[i].min[j]) && (DiceScore <= ORCData[i].max[j]))
				result = (CHAR_SKILL_RESULT)i;
			if ((SRS >= 10) && (DiceScore >= ORCData[i].min[19]) && (DiceScore <= ORCData[i].max[19]))
				result = (CHAR_SKILL_RESULT)i;
			if (result != NONE_CHAR_SKILL_RESULT)
				break;
		}
		if (result != NONE_CHAR_SKILL_RESULT)
			break;
	}

	if (result == NONE_CHAR_SKILL_RESULT)
		result = CRITICAL_FAILURE;
}

void CharSkill::GenerateXP(int *total)
{
	switch (result)
	{
	case CRITICAL_SUCCESS:
		xp += 2 * BASIC_XP;
		SuccessCount++;
		if (total) *total += 1;
		break;
	case SUCCESS:
		xp += BASIC_XP;
		SuccessCount++;
		if (total) *total += 1;
		break;
	}
}

void CharSkill::GeneratePenality(MsgReporter *pMsgReporter)
{
	int penality, MaxForLevel;
	float rate = 0;

	penality = 0;
	MaxForLevel = level * 10;

	//Plus la compétence a progressé avant la mort, moin la pénalité est grande
	if (BPData.total)
	{
		rate = (float)BASIC_XP / BPData.total * 100;
		if (rate < 1)
			rate = 1;
		if (rate > 100)
			rate = 100;
		// On atténue la pénalité en fontion du level (quand même faut pas pousser!)
		if (rate > MaxForLevel)
			rate = (float)MaxForLevel;
		penality = BPData.gain * (int)rate / 100;
	}

	if (penality)
	{
		BPData.gain -= penality;
		if (pMsgReporter) pMsgReporter->AddMessage(CURRENT_ACTION, BuildString("Vous perdez %d%% de vos points d'expérience dans la compétence %s", (int)rate, Description.Get()), XP_MSG_COLOR);
	}
}

CharSkill * CharSkill::Use()
{
	int i, StatBonus, SkillScore;

	used = true;
	StatBonus = 0;

	// On calcul le Skill Resolution Score (SRS).
	// Ref: "The Freestyle RPG Basic Rules", page 9.

	for (i = 0; i < StatPack.GetElementCount(); i++)
		StatBonus += StatPack.getCharStatElement(i)->GetModifier();

	StatBonus /= StatPack.GetElementCount();
	SkillScore = level + StatBonus;
	SRS = SkillScore - difficulty;

	return this;
}

CharSkill * CharSkill::Use(MagicSpell *pMagicSpell, CharStat *MagicalAffinity)
{
	int SchoolBonus;

	used = true;
	SchoolBonus = 0;

	/*Calcul le Skill Resolution Score (SRS) pour un sort.
	Ref: "The Freestyle RPG Basic Rules", page 24.*/

	if (pMagicSpell->GetSchool())
		SchoolBonus = StatPack.GetStat(pMagicSpell->GetSchool())->GetModifier();

	SRS = level + SchoolBonus + MagicalAffinity->GetModifier() - pMagicSpell->GetDifficulty();

	return this;
}

int CharSkill::GainXP(int OppositeLevel, int total, int MaxLevel, MsgReporter *pMsgReporter)
{
	EventData1 XPData;

	if ((xp > 0) && (SuccessCount > 0))
	{
		xp /= SuccessCount;
		if (total) xp *= (float)SuccessCount / total;
		XPData.score = (int)xp;
		if (OppositeLevel) xp -= (level - OppositeLevel) * BASIC_XP / 10;
		if (xp < 0) xp = 0;
		XPData.RealScore = (int)xp;
		SuccessCount = 0;
		xp = 0;
		if (level < MaxLevel)
		{
			BPData.gain += XPData.RealScore;
			BPData.total += XPData.RealScore;
			if (pMsgReporter)
				if (XPData.RealScore)
					pMsgReporter->AddMessage(CURRENT_ACTION, BuildString("Vous gagnez %d (%d) points d'expérience dans la compétence %s", XPData.RealScore, XPData.score, Description.Get()), XP_MSG_COLOR);
				else
					pMsgReporter->AddMessage(CURRENT_ACTION, BuildString("Vous ne gagnez pas d'expérience dans la compétence %s", Description.Get()), XP_MSG_COLOR);
		}
	}

	return XPData.RealScore;
}

bool CharSkill::ImproveLevel(int max)
{
	int i;

	GenerateLevel();

	//Si les points gagnés sont suffisants pour le niveau suivant, passe la skill au niveau suivant
	//Ref: "The Freestyle RPG Basic Rules", page 10.
	if ((BPData.gain >= BPData.level) && (BPData.level > 0) && (level < max))
	{
		level++;
		BPData.gain = BPData.total = 0;
		GenerateLevel();
		for (i = 0; i < StatPack.GetElementCount(); i++)
			if ((StatPack.getCharStatElement(i)->GetScore() < MAX_CHAR_STAT_SCORE) && (StatPack.getCharStatElement(i)->GetTempScore() == 0))
				StatPack.getCharStatElement(i)->SetTempScore(StatPack.getCharStatElement(i)->GetScore());
		return true;
	}

	return false;
}
