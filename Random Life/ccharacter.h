#ifndef CCHARACTER
#define CCHARACTER

#include <cvisual.h>
#include <cmover.h>
#include <ctileactor.h>
#include <cenergybar.h>
#include "cminievents.h"
#include "ccharskill.h"
#include "cweapon.h"
#include "ccharstatlist.h"
#include "ccharskilllist.h"
#include "main.h"

class Character : public Visual, virtual public Mover, virtual public TileActor
{
private:
	CharCombat Combat;
	List AttackedBy, IgnoredBy;
protected:
	int HPBonus;
	CHAR_RACE Race;
	CHAR_CLASS Class;
	CharBehavior Behavior;
	CharStatus Status;
	CharSkill MagicResistSkill, *CurrentSkill;
	Weapon *EquipedWp, CurrentWp;
	EventData2 CharEvent;
	EnergyBar *HitPoints;
	Character *Target, *AttackTarget;
	Str DisplayedName, Title;
	Clock Swatch[TOTAL_CHAR_SWATCH];
	Sound *GoSound, *AttackSound, *HitSound, *SleepSound;
	List EnchantPack;
	CharStatList StatPack, Stat2Pack;
	CharSkillList CombatSkillPack;
public:
	Character();
	Character(Character &model);
	~Character();
	virtual void SetTarget(Character *pCharacter);
	void SetAsOwnTarget() {if (!Status.attacking) Target = this;}
	void SetGoSound(Sound *pSound) {GoSound = pSound;}
	virtual bool Init();
	void InitOrientation();
	void initAni();
	virtual void InitSound(List *pSoundList) {}
	void restoreAni();
	void destroyAni();
	void GenerateDisplacement(TileEngine *pTileEngine, CardinalPoint *MaxDistance, CardinalPoint Obstacle[], int ObstacleCount, bool stop, bool rebound);
	virtual void GenerateStatus(short *MeditateProgress, MsgReporter *pMsgReporter=NULL, MiniEvents *pMiniEvents=NULL);
	void GenerateAni(List *pSoundList);
	void GenerateAniRotation();
	virtual void GenerateStat() {for (int i = 0; i < StatPack.GetElementCount(); i++) CharStat::getCharStatElement(i, &StatPack)->SetModifier();}
	virtual void GenerateStat2();
	virtual EVENT_TYPE GenerateSkill(ORC ORCData[], MsgReporter *pMsgReporter=NULL, MiniEvents *pMiniEvents=NULL);
	virtual EVENT_TYPE CheckActionResult(ORC ORCData[], MsgReporter *pMsgReporter=NULL, MiniEvents *pMiniEvents=NULL);
	void MagicSavingRoll(MagicSpell *pMagicSpell);
	bool UpdateCombat(CharCombat CombatData[]);
	virtual void EngageCombat(MsgReporter *pMsgReporter=NULL);
	virtual void ExitCombat(MsgReporter *pMsgReporter=NULL);
	virtual void WinCombat(Object *Corpse, MsgReporter *pMsgReporter=NULL, MiniEvents *pMiniEvents=NULL);
	virtual bool Attack();
	virtual EventData2 Hit(Weapon *pWeapon, MsgReporter *pMsgReporter=NULL);
	bool Wait(int index, int time, int TimeSpeed);
	void Sleep();
	void Recover(MsgReporter *pMsgReporter=NULL);
	virtual void Curse(bool value, short chao) {Status.cursed = value;}
	virtual void Diiiiiie(MsgReporter *pMsgReporter=NULL, MiniEvents *pMiniEvents=NULL);
	virtual void Reborn(bool start, MsgReporter *pMsgReporter=NULL);
	void PlaySound(Sound *pSound, DWORD repeats, bool RepeatOnce);
	void StopSound(Sound *pSound, bool OnlyIfRepeats=false);
	virtual void ShutUp() {if (Status.PlayingRepeatSound) StopSound(SleepSound);}
	int GetEnchant(PICKUP_ENCHANT EnchantEffect);
	bool HasNoTarget() {return Target == this;}
	int GetHPBonus() {return HPBonus;}
	CHAR_CLASS GetClass() {return Class;}
	CHAR_RACE GetRace() {return Race;}
	CharStatus * GetStatus() {return &Status;}
	CharBehavior * GetBehavior() {return &Behavior;}
	CharSkill * GetMagicResistSkill() {return &MagicResistSkill;}
	CharSkill * GetCurrentSkill() {return CurrentSkill;}
	CharCombat * GetCombat() {return &Combat;}
	Weapon * GetEquipedWp() {return EquipedWp;}
	Weapon * GetCurrentWp() {return &CurrentWp;}
	EnergyBar * GetHitPoints() {return HitPoints;}
	Str * GetTitle() {return &Title;}
	Clock * GetSwatch(int index) {return &Swatch[index];}
	Character * GetTarget() {return Target;}
	Character * GetAttackTarget() {return AttackTarget;}
	EventData2 * GetCharEvent() {return &CharEvent;}
	Sound * GetGoSound() {return GoSound;}
	Sound * GetAttackSound() {return AttackSound;}
	Sound * GetHitSound() {return HitSound;}
	Sound * GetSleepSound() {return SleepSound;}
	List * GetEnchantPack() {return &EnchantPack;}
	List * GetAttackedBy() {return &AttackedBy;}
	List * GetIgnoredBy() {return &IgnoredBy;}
	CharStatList * GetStatPack() {return &StatPack;}
	CharStatList * GetStat2Pack() {return &Stat2Pack;}
	CharSkillList * GetCombatSkillPack() {return &CombatSkillPack;}
	Str * GetDisplayedName() {return &DisplayedName;}
	static Character* getCharacterElement(int index, List *pList) {return dynamic_cast<Character*>(pList->GetElement(index));}
};

#endif