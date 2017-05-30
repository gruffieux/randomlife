#ifndef CMAGICSPELL
#define CMAGICSPELL

#include <cobject.h>
#include <cclock.h>
#include <csound.h>
#include "main.h"

class MagicSpell : public Object
{
private:
	int school, level, difficulty, MinDifficulty, CastTime;
	short CheckTurn;
	bool hit;
	CHAR_MAGIC_SPELL_EFFECT effect;
	FlagedData Duration, Range, MinEffect, MaxEffect;
	EventData1 ManaCost;
	EventData2 HitData;
	Str Description;
	Clock EffectDuration;
	Sound *HitSound;
public:
	MagicSpell();
	MagicSpell(MagicSpell &model);
	void SetSchool(int MagicSpell_school) {school = MagicSpell_school;}
	void SetLevel(int MagicSpell_level) {level = MagicSpell_level;}
	void SetDifficulty(int MagicSpell_difficulty) {difficulty = MagicSpell_difficulty;}
	void SetMinDifficulty(int MagicSpell_MinDifficulty) {MinDifficulty = MagicSpell_MinDifficulty;}
	void SetCastTime(int MagicSpell_CastTime) {CastTime = MagicSpell_CastTime;}
	void SetCheckTurn(short MagicSpell_CheckTurn) {CheckTurn = MagicSpell_CheckTurn;}
	void SetHit(bool MagicSpell_hit) {hit = MagicSpell_hit;}
	void SetEffect(CHAR_MAGIC_SPELL_EFFECT MagicSpell_effect) {effect = MagicSpell_effect;}
	void SetDescription(Str MagicSpell_Description) {Description = MagicSpell_Description;}
	void SetHitSound(Sound *pSound) {HitSound = pSound;}
	bool compare(Object *pObject);
	void IncreaseCastTime();
	void DecreaseCastTime();
	EventData1 GetManaCost(int WPModifier, bool generate);
	int GetSchool() {return school;}
	int GetLevel() {return level;}
	int GetDifficulty(){return difficulty;}
	int GetMinDifficulty() {return MinDifficulty;}
	int GetCastTime() {return CastTime;}
	short GetCheckTurn() {return CheckTurn;}
	bool GetHit() {return hit;}
	CHAR_MAGIC_SPELL_EFFECT GetEffect() {return effect;}
	FlagedData* GetDuration() {return &Duration;}
	FlagedData* GetRange() {return &Range;}
	FlagedData* GetMinEffect() {return &MinEffect;}
	FlagedData* GetMaxEffect() {return &MaxEffect;}
	EventData1* GetManaCost() {return &ManaCost;}
	EventData2* GetHitData() {return &HitData;}
	Str GetDescription() {return Description;}
	Clock* GetEffectDuration() {return &EffectDuration;}
	Sound* GetHitSound() {return HitSound;}
	static int magicSpellSchool(int effect);
	static MagicSpell* getMagicSpellElement(int index, List *pList) {return dynamic_cast<MagicSpell*>(pList->GetElement(index));}
};

#endif