#ifndef CPLAYER
#define CPLAYER

#include "chumanized.h"
#include "cingredient.h"
#include "cpotion.h"
#include "cmob.h"
#include "cpnj.h"

class Player : public Humanized
{
private:
	int SkillPoints, GlobalHitCount, GlobalHittedCount;
	short NameID;
	bool AlwaysWalk;
	Ingredient *PotionElement[TOTAL_PICKUP_POTION_ELEMENT];
	Potion *Crafting;
	Mob *MobTarget;
	Pnj *PnjTarget;
	EnergyBar *MagicSkillBar;
	List CraftPack, CombatSkillBarPack, SkillBarPack;
public:
	Player();
	Player(Player &model);
	~Player();
	void SetSkillPoints(int Player_SkillPoints) {SkillPoints = Player_SkillPoints;}
	void SetNameID(short Player_NameID) {NameID = Player_NameID;}
	void SetCrafting(Potion *pPotion) {Crafting = pPotion;}
	void SetTarget(Character *pCharacter);
	void SwitchAlwaysWalk() {AlwaysWalk = !AlwaysWalk;}
	bool Init();
	void initAni();
	void restoreAni();
	void destroyAni();
	void AddPotionElement(int index, Ingredient *pIngredient);
	EVENT_TYPE GenerateSkill(ORC ORCData[], MsgReporter *pMsgReporter=NULL, MiniEvents *pMiniEvents=NULL);
	void GenerateName();
	void Curse(bool value, short chao) {Status.cursed = false;}
	EVENT_TYPE LearnSpell(MagicSpell *pMagicSpell, List SpellPack, MsgReporter *pMsgReporter=NULL);
	EVENT_TYPE Drop(Pickup *pPickup, MsgReporter *pMsgReporter=NULL);
	EVENT_TYPE CheckActionResult(ORC ORCData[], MsgReporter *pMsgReporter=NULL, MiniEvents *pMiniEvents=NULL);
	void CraftPotion(MsgReporter *pMsgReporter);
	EVENT_TYPE Give(Pickup *pPickup, Humanized *pHumanized, MsgReporter *pMsgReporter=NULL, MiniEvents *pMiniEvents=NULL);
	bool Consume(Potion *pPotion, List *PickupPack, MsgReporter *pMsgReporter=NULL);
	void StopSpellEffect(MagicSpell *pMagicSpell, MsgReporter *pMsgReporter=NULL, MiniEvents *pMiniEvents=NULL);
	void EngageCombat(MsgReporter *pMsgReporter=NULL);
	void ExitCombat(MsgReporter *pMsgReporter=NULL);
	void WinCombat(Object *Corpse, MsgReporter *pMsgReporter=NULL, MiniEvents *pMiniEvents=NULL);
	void Undo() {Status.look = Status.spy = Status.trading = false;}
	void Reborn(bool start, MsgReporter *pMsgReporter=NULL);
	int GetSkillPoints() {return SkillPoints;}
	int* GetpSkillPoints() {return &SkillPoints;}
	int GetGlobalHitCount() {return GlobalHitCount;}
	int GetGlobalHittedCount() {return GlobalHittedCount;}
	short GetNameID() {return NameID;}
	bool GetAlwaysWalk() {return AlwaysWalk;}
	Ingredient* getPotionElement(int index) {return PotionElement[index];}
	Potion* GetCrafting() {return Crafting;}
	Mob* GetMobTarget();
	Pnj* GetPnjTarget();
	EnergyBar* GetMagicSkillBar() {return MagicSkillBar;}
	List* GetCraftPack() {return &CraftPack;}
	List* GetCombatSkillBarPack() {return &CombatSkillBarPack;}
	List* GetSkillBarPack() {return &SkillBarPack;}
	static Player* getPlayerElement(int index, List *pList) {return dynamic_cast<Player*>(pList->GetElement(index));}
};

#endif