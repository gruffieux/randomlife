#ifndef CHUMANIZED
#define CHUMANIZED

#include <ctileia.h>
#include "ccharacter.h"
#include "carmor.h"
#include "cpickuplist.h"
#include "cmagicspelllist.h"
#include "cpotion.h"

class Humanized : public Character
{
private:
	int purse, PotionEffectDuration;
	EnergyBar *ChargePoints;
	TileIA *TileIATarget;
protected:
	int MagicPoints;
	CHAR_BEHAVIOR PrimaryBehavior;
	CharSkill MagicSkill;
	EnergyBar *ManaPoints;
	MagicSpell CurrentSpell, *MemorizedSpell;
	Pickup *Stealing;
	Armor *EquipedArmor;
	Character *SpellTarget;
	Humanized *HumanizedTarget;
	List PotionGift;
	PickupList Pocket[TOTAL_PICKUP_GROUP];
	CharStatList MagicSchoolPack;
	CharSkillList SkillPack;
	MagicSpellList *MagicSpellPack;
public:
	Humanized();
	Humanized(Humanized &model);
	~Humanized();
	void SetMagicPoints(int Humanized_MagicPoints) {MagicPoints = Humanized_MagicPoints;}
	void SetPrimaryBehavior();
	void SetMemorizedSpell(MagicSpell *pMagicSpell);
	void SetSpellTarget(Character *pCharacter);
	void SetStealing(Pickup *pPickup) {Stealing = pPickup;}
	void SetTarget(Character *pCharacter);
	bool Init();
	void initAni();
	void MapSkins();
	void InitSound(List *pSoundList);
	void restoreAni();
	void destroyAni();
	void GenerateStatus(short *MeditateProgress, MsgReporter *pMsgReporter=NULL, MiniEvents *pMiniEvents=NULL);
	void GenerateStat();
	void GenerateStat2();
	EVENT_TYPE GenerateSkill(ORC ORCData[], MsgReporter *pMsgReporter=NULL, MiniEvents *pMiniEvents=NULL);
	void GenerateTitle();
	void GenerateEquipment();
	EVENT_TYPE AddToEquipment(Pickup *pPickup, MsgReporter *pMsgReporter=NULL, MiniEvents *pMiniEvents=NULL);
	void AddStatMalus(Pickup *pPickup);
	void AddAllStatMalus();
	void RemoveStatMalus(Pickup *pPickup);
	void RemoveAllStatMalus();
	void UpdateMagicSpell();
	virtual bool Consume(Potion *pPotion, List *PickupPack, MsgReporter *pMsgReporter=NULL);
	EVENT_TYPE Equip(Pickup *pPickup, MsgReporter *pMsgReporter=NULL);
	EVENT_TYPE UnEquip(Pickup *pPickup, bool EvenSummoned, MsgReporter *pMsgReporter=NULL);
	virtual EVENT_TYPE Give(Pickup *pPickup, Humanized *pHumanized, MsgReporter *pMsgReporter=NULL, MiniEvents *pMiniEvents=NULL);
	EVENT_TYPE Buy(Pickup *pPickup, Humanized *Seller, List *PickupPack, MsgReporter *pMsgReporter=NULL, MiniEvents *pMiniEvents=NULL);
	void EmptyBag();
	virtual void CheckEquipment(MsgReporter *pMsgReporter=NULL) {;}
	EVENT_TYPE CheckActionResult(ORC ORCData[], MsgReporter *pMsgReporter=NULL, MiniEvents *pMiniEvents=NULL);
	void Meditate();
	void Spy();
	void BeDiscret();
	void Steal(Pickup *pPickup);
	bool Attack();
	bool Concentrate(bool RingIfFailure, MsgReporter *pMsgReporter=NULL, MiniEvents *pMiniEvents=NULL);
	void CastSpell();
	EventData2 Hit(Weapon *pWeapon, MsgReporter *pMsgReporter=NULL);
	void SpellHit(MagicSpell *pMagicSpell, MsgReporter *pMsgReporter=NULL, MiniEvents *pMiniEvents=NULL);
	void ReduceDamage(float *damage, MsgReporter *pMsgReporter=NULL);
	void AbsorbMagicEffect(float *effect);
	virtual void StopSpellEffect(MagicSpell *pMagicSpell, MsgReporter *pMsgReporter=NULL, MiniEvents *pMiniEvents=NULL);
	void Curse(bool value, short chao);
	void Diiiiiie(MsgReporter *pMsgReporter=NULL, MiniEvents *pMiniEvents=NULL);
	void Reborn(bool start, MsgReporter *pMsgReporter=NULL);
	void ShutUp();
	bool IsEmpty();
	bool IsAPlant() {return !((Behavior.progress == true) | (Behavior.move == true) | (Behavior.GetPickup == true));}
	EVENT_TYPE SummonNecroMisery(MsgReporter *pMsgReporter=NULL);
	Str GetMappedSkin(int skinId);
	int GetPurse() {return purse;}
	int GetPotionEffectDuration() {return PotionEffectDuration;}
	int GetMagicPoints() {return MagicPoints;}
	int* GetpMagicPoints() {return &MagicPoints;}
	CHAR_BEHAVIOR GetPrimaryBehavior() {return PrimaryBehavior;}
	CharSkill* GetMagicSkill() {return &MagicSkill;}
	MagicSpell* GetMemorizedSpell() {return MemorizedSpell;}
	MagicSpell* GetCurrentSpell() {return &CurrentSpell;}
	EnergyBar* GetManaPoints() {return ManaPoints;}
	EnergyBar* GetChargePoints() {return ChargePoints;}
	Armor* GetEquipedArmor() {return EquipedArmor;}
	Pickup* GetStealing() {return Stealing;}
	Character* GetSpellTarget() {return SpellTarget;}
	Humanized* GetHumanizedTarget();
	TileIA* GetTileIATarget();
	PickupList* GetPocket(int group) {return &Pocket[group];}
	List* GetPotionGift() {return &PotionGift;}
	CharStatList* GetMagicSchoolPack() {return &MagicSchoolPack;}
	CharSkillList* GetSkillPack() {return &SkillPack;}
	MagicSpellList* GetMagicSpellPack(int index) {return &MagicSpellPack[index];}
	static Humanized* getHumanizedElement(int index, List *pList) {return dynamic_cast<Humanized*>(pList->GetElement(index));}
};

#endif