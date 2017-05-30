#include "cmagicspell.h"

MagicSpell::MagicSpell()
{
	school = level = difficulty = MinDifficulty = CastTime = CheckTurn = 0;
	hit = false;
	effect = NONE_CHAR_MAGIC_SPELL_EFFECT;
	HitSound = NULL;
}

MagicSpell::MagicSpell(MagicSpell &model)
{
	school = model.GetSchool();
	level = model.GetLevel();
	difficulty = model.GetDifficulty();
	MinDifficulty = model.GetMinDifficulty();
	CastTime = model.GetCastTime();
	CheckTurn = model.GetCheckTurn();
	hit = model.GetHit();
	effect = model.GetEffect();
	Duration = *model.GetDuration();
	Range = *model.GetRange();
	MinEffect = *model.GetMinEffect();
	MaxEffect = *model.GetMaxEffect();
	ManaCost = *model.GetManaCost();
	HitData = *model.GetHitData();
	Description = model.GetDescription();
	EffectDuration = *model.GetEffectDuration();
	HitSound = model.GetHitSound();
}

bool MagicSpell::compare(Object *pObject)
{
	MagicSpell *pMagicSpell = dynamic_cast<MagicSpell*>(pObject);
	if (pMagicSpell) return ((school == pMagicSpell->GetSchool()) && (level == pMagicSpell->GetLevel()) && (effect == pMagicSpell->GetEffect()));
	return false;
}

void MagicSpell::IncreaseCastTime()
{
	if (difficulty > MinDifficulty)
	{
		CastTime++;
		difficulty--;
	}
}

void MagicSpell::DecreaseCastTime()
{
	if (CastTime > 2)
	{
		CastTime--;
		difficulty++;
	}
}

EventData1 MagicSpell::GetManaCost(int WPModifier, bool generate)
{
	EventData1 TempResult;

	TempResult.score = level + difficulty;
	TempResult.RealScore = TempResult.score - WPModifier;

	if (TempResult.RealScore <= 0)
		TempResult.RealScore = 1;

	if (generate)
		ManaCost = TempResult;

	return TempResult;
}

int MagicSpell::magicSpellSchool(int effect)
{
	switch (effect)
	{
		case SELF_HEAL:		return CURE;
		case LIFE_DAMAGE:	return DESTRUCTION;
		case CHOC_WAVE:		return DESTRUCTION;
		case ANTIMAGIC:		return DESTRUCTION;
		case ABSORBING:		return PROTECTION;
		case REFLECTING:	return PROTECTION;
		default:			return NONE_CHAR_MAGIC_SCHOOL;
	}
}
