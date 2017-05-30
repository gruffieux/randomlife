#ifndef CCREATURE
#define CCREATURE

#include "chumanized.h"
#include "cmob.h"

class Creature : public Humanized, public Mob
{
private:
public:
	Creature() {Race = HUMANIZED;}
	Creature(CHAR_CLASS Char_Class, CHAR_BEHAVIOR Char_Behavior, bool CharB_CurseInfluenced, bool CharB_move, bool CharB_dispatch, bool CharB_GetPickup, CHAR_SKILL CharB_FavoriteSkill1, CHAR_SKILL CharB_FavoriteSkill2, CHAR_COMBAT_SKILL CharB_FavoriteCombatSkill, PICKUP_GROUP CharB_ThiefType, Str Object_Name);
	Creature(Creature &model) {}
	~Creature() {}
	void CheckEquipment(MsgReporter *pMsgReporter=NULL);
	bool CheckDest(Object *pObject);
	EVENT_TYPE Give(Pickup *pPickup, Humanized *pHumanized, MsgReporter *pMsgReporter=NULL, MiniEvents *pMiniEvents=NULL);
	bool StopMove() {return TileIA::StopMove();}
	bool Attack();
	bool LookForStealing();
	bool MemorizeSpell(int school);
	void WinCombat(Object *Corpse, MsgReporter *pMsgReporter=NULL, MiniEvents *pMiniEvents=NULL);
	void Diiiiiie(int ZoneCoordX, int ZoneCoordY, MsgReporter *pMsgReporter=NULL, MiniEvents *pMiniEvents=NULL);
	static Creature* getCreatureElement(int index, List *pList) {return dynamic_cast<Creature*>(pList->GetElement(index));}
};

#endif