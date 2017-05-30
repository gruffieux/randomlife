#ifndef CANIMAL
#define CANIMAL

#include "ccharacter.h"
#include "cmob.h"

class Animal : public Character, public Mob
{
private:
	int DamageFactor;
	CHAR_ANIMAL_TYPE Type;
public:
	Animal();
	Animal(CHAR_ANIMAL_TYPE Animal_Type, int Animal_DamageFactor, int CharHPBonus, CHAR_CLASS Char_Class, bool CharB_aggro, bool CharB_move, bool CharB_dispatch, CHAR_COMBAT_SKILL CharB_FavoriteCombatSkill, Str Object_Name);
	Animal(Animal &model) {}
	~Animal() {}
	void initAni();
	void InitSound(List *pSoundList);
	bool CheckDest(Object *pObject);
	bool StopMove() {return TileIA::StopMove();}
	bool Attack();
	void Curse(bool value, short chao) {Status.cursed = false;}
	void WinCombat(Object *Corpse, MsgReporter *pMsgReporter=NULL, MiniEvents *pMiniEvents=NULL);
	void Diiiiiie(int ZoneCoordX, int ZoneCoordY, MsgReporter *pMsgReporter=NULL, MiniEvents *pMiniEvents=NULL);
	int GetDamageFactor() {return DamageFactor;}
	int GetType() {return Type;}
	static Animal* getAnimalElement(int index, List *pList) {return dynamic_cast<Animal*>(pList->GetElement(index));}
};

#endif