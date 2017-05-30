#ifndef CCHARSKILL
#define CCHARSKILL

#include <cobject.h>
#include <cmsgreporter.h>
#include "ccharstatlist.h"
#include "cmagicspell.h"
#include "main.h"

class CharSkill : public Object
{
private:
	int SkillId, level, difficulty, SRS, SuccessCount;
	float xp;
	bool used;
	CHAR_SKILL_RESULT result;
	BonusPoints BPData;
	Str Description;
	CharStatList StatPack;
public:
	CharSkill();
	CharSkill(CharSkill &model);
	~CharSkill() {StatPack.RemoveAllElement(); /*On enlève tout pour que pList soit NULL! (Les objets sont détruits dans Character!)*/}
	void SetSkillId(int CharSkill_SkillId) {SkillId = CharSkill_SkillId;}
	void SetLevel(int CharSkill_level) {level = CharSkill_level;}
	void SetDifficulty(int CharSkill_difficulty) {difficulty = CharSkill_difficulty;}
	void SetSRS(int CharSkill_SRS) {SRS = CharSkill_SRS;}
	void SetXP(float CharSkill_xp) {xp = CharSkill_xp;}
	void SetSuccessCount(int CharSkill_SuccessCount) {SuccessCount = CharSkill_SuccessCount;}
	void SetUsed(bool CharSkill_used) {used = CharSkill_used;}
	void SetDescription(Str CharSkill_Description) {Description = CharSkill_Description;}
	void InitXP();
	void GenerateLevel() {BPData.level = 10 * difficulty * (level + 1);}	//Définit le nombre de points à gagner pour passer une skill au niveau suivant
																			//Ref: "The Freestyle RPG Basic Rules", page 10.
	void GenerateResult(ORC ORCData[]);
	void GenerateXP(int *total);
	void GeneratePenality(MsgReporter *pMsgReporter=NULL);
	CharSkill* Use();
	CharSkill* Use(MagicSpell *pMagicSpell, CharStat *MagicalAffinity);
	int GainXP(int OppositeLevel, int total, int MaxLevel, MsgReporter *pMsgReporter=NULL);
	bool ImproveLevel(int max);
	int GetSkillId() {return SkillId;}
	int GetLevel() {return level;}
	int GetDifficulty() {return difficulty;}
	int GetSRS() {return SRS;}
	float GetXP() {return xp;}
	int GetSuccessCount() {return SuccessCount;}
	bool GetUsed() {return used;}
	CHAR_SKILL_RESULT GetResult() {return result;}
	BonusPoints* GetBPData() {return &BPData;}
	Str GetDescription() {return Description;}
	CharStatList* GetStatPack() {return &StatPack;}
	static CharSkill* getCharSkillElement(int index, List *pList) {return dynamic_cast<CharSkill*>(pList->GetElement(index));}
};


#endif