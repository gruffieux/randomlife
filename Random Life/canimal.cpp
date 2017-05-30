#include <canimation.h>
#include "canimal.h"
#include "../RLRes/RLRes/resource.h"

Animal::Animal()
{
	DamageFactor = 0;
	Type = FLAT_SNAKE;
	Race = ANIMAL;
	Behavior.FavoriteCombatSkill = NONE_CHAR_COMBAT_SKILL;
}

Animal::Animal(CHAR_ANIMAL_TYPE Animal_Type, int Animal_DamageFactor, int CharHPBonus, CHAR_CLASS Char_Class, bool CharB_aggro, bool CharB_move, bool CharB_dispatch, CHAR_COMBAT_SKILL CharB_FavoriteCombatSkill, Str Object_Name) :
Item(NULL, NULL, NULL, NULL, Object_Name)
{
	DamageFactor = Animal_DamageFactor;
	Type = Animal_Type;
	HPBonus = CharHPBonus;
	Race = ANIMAL;
	Class = Char_Class;
	Behavior.aggro = CharB_aggro;
	Behavior.move = CharB_move;
	Behavior.dispatch = CharB_dispatch;
	Behavior.FavoriteCombatSkill = CharB_FavoriteCombatSkill;
}

void Animal::initAni()
{
	int i;
	Character::initAni();

	switch (Type)
	{
	case SPIDER:
		addAni(IDB_SPIDER_STOP, "", 1, 14, 11, RGB(255, 255, 255), 90);
		addAni(IDB_SPIDER_MOVE, "", 2, 10, 8, RGB(255, 255, 255), 90);
		for (i = 0; i < 4; i++)
			Animation::getAnimationElement(1, &animations)->SetOriginPoint(i, 2, 1);
		addAni(IDB_SPIDER_STOP, "", 1, 14, 11, RGB(255, 255, 255), 90);
		addAni(IDB_SPIDER_STOP, "", 1, 14, 11, RGB(255, 255, 255), 90);
		addAni(IDB_SPIDER_DEAD, "", 1, 10, 9, RGB(255, 255, 255), 90);
		for (i = 0; i < 4; i++)
			Animation::getAnimationElement(4, &animations)->SetOriginPoint(i, 2, 2);
		addAni(IDB_SPIDER_ATTACK_HTH, "", 5, 14, 11, RGB(255, 255, 255), 90);
		break;
	case FLAT_SNAKE:
		addAni(IDB_FLATSNAKE_STOP, "", 1, 9, 25, RGB(255, 255, 255), 90);
		addAni(IDB_FLATSNAKE_MOVE, "", 4, 9, 25, RGB(255, 255, 255), 90);
		addAni(IDB_FLATSNAKE_HITTED, "", 4, 9, 25, RGB(255, 255, 255), 90);
		addAni(IDB_FLATSNAKE_SLEEP, "", 1, 18, 15, RGB(255, 255, 255), 90);
		Animation::getAnimationElement(3, &animations)->SetOriginPoint(0, -5, 5);
		Animation::getAnimationElement(3, &animations)->SetOriginPoint(1, 0, -5);
		Animation::getAnimationElement(3, &animations)->SetOriginPoint(2, -5, 5);
		Animation::getAnimationElement(3, &animations)->SetOriginPoint(3, 5, -5);
		addAni(IDB_FLATSNAKE_DEAD, "", 4, 9, 25, RGB(255, 255, 255), 90);
		addAni(IDB_FLATSNAKE_ATTACK_HTH, "", 6, 9, 40, RGB(255, 255, 255), 90);
		Animation::getAnimationElement(5, &animations)->SetOriginPoint(0, 0, -15);
		Animation::getAnimationElement(5, &animations)->SetOriginPoint(1, -15, 0);
		break;
	case PINK_SNAKE:
		addAni(IDB_PINKSNAKE_STOP, "", 1, 9, 25, RGB(255, 255, 255), 90);
		addAni(IDB_PINKSNAKE_MOVE, "", 4, 9, 25, RGB(255, 255, 255), 90);
		addAni(IDB_PINKSNAKE_HITTED, "", 4, 9, 25, RGB(255, 255, 255), 90);
		addAni(IDB_PINKSNAKE_SLEEP, "", 1, 18, 15, RGB(255, 255, 255), 90);
		Animation::getAnimationElement(3, &animations)->SetOriginPoint(0, -5, 5);
		Animation::getAnimationElement(3, &animations)->SetOriginPoint(1, 0, -5);
		Animation::getAnimationElement(3, &animations)->SetOriginPoint(2, -5, 5);
		Animation::getAnimationElement(3, &animations)->SetOriginPoint(3, 5, -5);
		addAni(IDB_PINKSNAKE_DEAD, "", 4, 9, 25, RGB(255, 255, 255), 90);
		addAni(IDB_PINKSNAKE_ATTACK_HTH, "", 6, 9, 40, RGB(255, 255, 255), 90);
		Animation::getAnimationElement(5, &animations)->SetOriginPoint(0, 0, -15);
		Animation::getAnimationElement(5, &animations)->SetOriginPoint(1, -15, 0);
		break;
	case BLACK_BEAR:
		addAni(IDB_BEAR_STOP, "", 1, 22, 30, RGB(255, 255, 255), 90);
		addAni(IDB_BEAR_MOVE, "", 10, 22, 30, RGB(255, 255, 255), 90);
		addAni(IDB_BEAR_HITTED, "", 4, 22, 30, RGB(255, 255, 255), 90);
		Animation::getAnimationElement(2, &animations)->SetOriginPoint(0, 0, 1);
		Animation::getAnimationElement(2, &animations)->SetOriginPoint(1, 1, -1);
		Animation::getAnimationElement(2, &animations)->SetOriginPoint(2, 0, -1);
		Animation::getAnimationElement(2, &animations)->SetOriginPoint(3, -1, -1);
		addAni(IDB_BEAR_SLEEP, "", 1, 22, 30, RGB(255, 255, 255), 90);
		addAni(IDB_BEAR_DEAD, "", 4, 26, 30, RGB(255, 255, 255), 90);
		Animation::getAnimationElement(4, &animations)->SetOriginPoint(0, -2, 0);
		Animation::getAnimationElement(4, &animations)->SetOriginPoint(1, -1, 0);
		Animation::getAnimationElement(4, &animations)->SetOriginPoint(2, -2, 0);
		Animation::getAnimationElement(4, &animations)->SetOriginPoint(3, 1, 0);
		addAni(IDB_BEAR_ATTACK_HTH, "", 7, 22, 35, RGB(255, 255, 255), 90);
		Animation::getAnimationElement(5, &animations)->SetOriginPoint(0, 0, -5);
		Animation::getAnimationElement(5, &animations)->SetOriginPoint(1, -5, 0);
		break;
	case MALEFIC_DOG:
		addAni(IDB_DOG_STOP, "", 1, 9, 19, RGB(255, 255, 255), 90);
		addAni(IDB_DOG_MOVE, "", 5, 9, 19, RGB(255, 255, 255), 90);
		addAni(IDB_DOG_HITTED, "", 6, 9, 19, RGB(255, 255, 255), 90);
		addAni(IDB_DOG_SLEEP, "", 1, 9, 19, RGB(255, 255, 255), 90);
		addAni(IDB_DOG_DEAD, "", 4, 9, 19, RGB(255, 255, 255), 90);
		addAni(IDB_DOG_ATTACK_HTH, "", 5, 9, 22, RGB(255, 255, 255), 90);
		Animation::getAnimationElement(5, &animations)->SetOriginPoint(0, 0, -3);
		Animation::getAnimationElement(5, &animations)->SetOriginPoint(1, -3, 0);
		break;
	}
}

void Animal::InitSound(List *pSoundList)
{
	switch (Type)
	{
	case SPIDER:
		HitSound = (Sound*)pSoundList->GetElement(HUMAN_SCREAM);
		break;
	case FLAT_SNAKE:
		AttackSound = (Sound*)pSoundList->GetElement(SNAKE_ATTACK);
		HitSound = (Sound*)pSoundList->GetElement(ANIMAL_BITE);
		break;
	case PINK_SNAKE:
		AttackSound = (Sound*)pSoundList->GetElement(SNAKE_ATTACK);
		HitSound = (Sound*)pSoundList->GetElement(ANIMAL_BITE);
		break;
	case BLACK_BEAR:
		AttackSound = (Sound*)pSoundList->GetElement(HUMAN_ATTACK);
		HitSound = (Sound*)pSoundList->GetElement(HUMAN_SCREAM);
		break;
	case MALEFIC_DOG:
		AttackSound = (Sound*)pSoundList->GetElement(DOG_ATTACK);
		HitSound = (Sound*)pSoundList->GetElement(ANIMAL_BITE);
		break;
	}
}

bool Animal::CheckDest(Object *pObject)
{
	Character *pCharacter = dynamic_cast<Character*>(pObject);

	if (!pCharacter) return false;
	if (pCharacter->getRemoved()) return false;
	if (pCharacter->GetRace() == ANIMAL) return false;
	if (pCharacter->GetStatus()->cursed) return false;
	if (pCharacter->GetStatus()->invisible) return false;
	if (pCharacter->GetStatus()->dead) return false;
	if (pCharacter->GetIgnoredBy()->SearchElement(this) != -1) return false;

	return TileIA::CheckDest(pCharacter);
}

bool Animal::Attack()
{
	if (CanAttack(Target))
	{
		Character::Attack();
		CurrentWp.SetMinDamage(CurrentWp.GetMinDamage() * DamageFactor);
		CurrentWp.SetMaxDamage(CurrentWp.GetMaxDamage() * DamageFactor);
		return true;
	}

	return false;
}

void Animal::WinCombat(Object *Corpse, MsgReporter *pMsgReporter, MiniEvents *pMiniEvents)
{
	Character::WinCombat(Corpse, pMsgReporter, pMiniEvents);
	Mob::WinCombat(Corpse, pMsgReporter, pMiniEvents);
}

void Animal::Diiiiiie(int ZoneCoordX, int ZoneCoordY, MsgReporter *pMsgReporter, MiniEvents *pMiniEvents)
{
	Character::Diiiiiie(pMsgReporter, pMiniEvents);
	Mob::Diiiiiie(ZoneCoordX, ZoneCoordY, pMsgReporter, pMiniEvents);
}
