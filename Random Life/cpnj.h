#ifndef CPNJ
#define CPNJ

#include "chumanized.h"
#include "cmob.h"

class Pnj : public Humanized, public Mob
{
private:
	CHAR_PNJ_JOB PNJJob;
	CHAR_PNJ_QUEST PNJQuest;
public:
	Pnj() {Race = HUMANIZED;}
	Pnj(CHAR_CLASS Char_Class, CHAR_PNJ_JOB Char_PNJJob, CHAR_PNJ_QUEST Char_PNJQUest, bool CharB_dispatch, bool CharB_GetPickup, CHAR_SKILL CharB_FavoriteSkill1, CHAR_SKILL CharB_FavoriteSkill2, CHAR_COMBAT_SKILL CharB_FavoriteCombatSkill, Str Object_Name);
	Pnj(Pnj &model);
	~Pnj() {}
	bool CheckDest(Object *pObject);
	void CheckEquipment(MsgReporter *pMsgReporter=NULL);
	EVENT_TYPE Give(Pickup *pPickup, Humanized *pHumanized, MsgReporter *pMsgReporter=NULL, MiniEvents *pMiniEvents=NULL);
	bool StopMove() {return TileIA::StopMove();}
	bool Attack();
	void Curse(bool value, short chao);
	void WinCombat(Object *Corpse, MsgReporter *pMsgReporter=NULL, MiniEvents *pMiniEvents=NULL);
	void Diiiiiie(int ZoneCoordX, int ZoneCoordY, MsgReporter *pMsgReporter=NULL, MiniEvents *pMiniEvents=NULL);
	CHAR_PNJ_JOB GetPNJJob() {return PNJJob;}
	CHAR_PNJ_QUEST GetPNJQuest() {return PNJQuest;}
	static Pnj* getPnjElement(int index, List *pList) {return dynamic_cast<Pnj*>(pList->GetElement(index));}
};

#endif